const express = require('express');
const axios = require('axios');
const cors = require('cors');
const app = express();

app.use(cors());
app.use(express.json());

const OLLAMA_API_URL = 'http://localhost:11434/api/chat';

const chatHistories = new Map();

function getClientIp(req){
    return req.headers['x-forwarded-for'] || req.connection.remoteAddress;
}

app.post('/chat', async (req, res) => {

    const ip = getClientIp(req);

    const message = req.body.prompt;

    if (!chatHistories.has(ip)) {
        chatHistories.set(ip, [
          {
            role: 'system',
            content: 'You are a helpful Doctor assistant.Your name is Dr.Jack.'
          }
        ]);
    }

    const chatHistory = chatHistories.get(ip);
    chatHistory.push({ role: 'user', content: message });

    try {

        const response = await axios.post(OLLAMA_API_URL, {
            model: 'llama3.2',
            messages: chatHistory,
            stream: false,
        });

        const reply = response.data.message.content;

        chatHistory.push({ role: 'assistant', content: reply });
        console.log('reply:', chatHistory);
        res.json({ response: reply });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

app.listen(3000, () => console.log('🧠 Chatbot backend running on http://localhost:3000'));