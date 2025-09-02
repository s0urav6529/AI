import asyncio
import streamlit as st
from langchain_mcp_adapters.client import MultiServerMCPClient
from langchain.chat_models import init_chat_model
from langchain_openai import ChatOpenAI
from langchain_ollama import ChatOllama
from langgraph.graph import StateGraph, MessagesState, START, END
from langgraph.prebuilt import ToolNode
import os
from dotenv import load_dotenv

load_dotenv()

openai_api_key = os.getenv("OPENAI_API_KEY")
mcp_server_script_path = os.getenv("mcp_server_script_path")
ollama_base_url = os.getenv("ollama_base_url")
ai_model = os.getenv("ai_model")

if not openai_api_key:
    raise ValueError("OPENAI_API_KEY environment variable not set")

async def run_mcp_query(user_input):
    
    # Initialize the ChatOpenAI model with the API key
    # model = ChatOpenAI(model="gpt-4", temperature=0.7, api_key=openai_api_key)
    model = ChatOllama(model=ai_model, temperature=0.7, base_url=ollama_base_url)
    
    # Initialize the MultiServerMCPClient with the model
    client = MultiServerMCPClient(
        {
            "multi_tool_mcp":{
                "command": "python",
                "args": [mcp_server_script_path], # path to your MCP server script
                "transport": "stdio", # if server & client are on same machine
            },
            
            # if mcp server on remote location
            # "multi_tool_mcp":{
            #     "transport": "streamable-http",
            #     "url": "http://<remote-ip>:<port>",
            # }
        }
    )
    
    tools = await client.get_tools()
    model_with_tools = model.bind_tools(tools)
    tool_node = ToolNode(tools)
    
    def should_continue(state: MessagesState):
        messages = state["messages"]
        last_message = messages[-1]
        if last_message.tool_calls:
            return "tools"
        return END
    
    async def call_model(state: MessagesState):
        messages = state["messages"]
        response = await model_with_tools.ainvoke(messages)
        # state["messages"].append(response)
        return { "messages": [response] }
    
    builder = StateGraph(MessagesState)
    builder.add_node("call_model", call_model)
    builder.add_node("tools", tool_node)
    
    builder.add_edge(START, "call_model")
    builder.add_conditional_edges("call_model", should_continue)
    builder.add_edge("tools", "call_model")
    
    
    # Compile the graph
    graph = builder.compile()
    
    result = await graph.ainvoke({ "messages":[{"role": "user", "content": user_input}]})
    
    # Extract lass message content
    last_message = result["messages"][-1].content
    return last_message if isinstance(last_message, str) else str(last_message)


def main():
    st.set_page_config(page_title="MCP with LangGraph", page_icon="🤖")
    st.title("Hi i am your Multi-MCP Bot 🤖")
    
    user_input = st.text_input("Ask me anything:")
    if st.button("Send") and user_input.strip():
        with st.spinner("Thinking..."):
            answer = asyncio.run(run_mcp_query(user_input))
            st.success(answer)

if __name__ == "__main__":
    main()