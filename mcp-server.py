from mcp.server.fastmcp import FastMCP
from datetime import datetime
import math
import pytz

# Initialize FastMCP server
mcp = FastMCP("multi_tool_mcp")


# ---------------- Calculator Tools ----------------
@mcp.tool()
def add(a: int, b: int) -> int:
    return a + b

@mcp.tool()
def subtract(a: int, b: int) -> int:
    return a - b

@mcp.tool()
def multiply(a: int, b: int) -> int:
    return a * b

@mcp.tool()
def divide(a: int, b: int) -> float:
    if b == 0:
        raise ValueError("Cannot divide by zero.")
    return a / b

@mcp.tool()
def square(a: int) -> int:
    return multiply(a, a)

@mcp.tool()
def square_root(a: int) -> float:
    if a < 0:
        raise ValueError("Cannot take square root of negative number.")
    return math.sqrt(a)

@mcp.tool()
def power(a: int, b: int) -> float:
    return a ** b

@mcp.tool()
def percentage(a: int, b: int) -> float:
    if b == 0:
        raise ValueError("Cannot calculate percentage with denominator zero.")
    return (a / b) * 100

@mcp.tool()
def factorial(n: int) -> int:
    if n < 0:
        raise ValueError("Cannot compute factorial of negative number.")
    return math.factorial(n)

@mcp.tool()
def fibonacci(n: int) -> int:
    if n < 0:
        raise ValueError("Cannot compute Fibonacci of negative number.")
    a, b = 0, 1
    for _ in range(n):
        a, b = b, add(a, b)
    return a


# ---------------- Timezone Tool ----------------
@mcp.tool()
def get_time(timezone: str) -> str:
    """Return current time in given timezone."""
    try:
        tz = pytz.timezone(timezone)
    except Exception:
        raise ValueError("Invalid timezone. Example: 'Asia/Dhaka'")
    return datetime.now(tz).strftime("%Y-%m-%d %H:%M:%S")


if __name__ == "__main__":
     mcp.run(transport="stdio") # Use stdio transport for local communication
    # mcp.run(transport="streamable-http") # Use streamable-http transport for remote communication
