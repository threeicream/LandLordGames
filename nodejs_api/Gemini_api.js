// 引入 Google Generative AI (Gemini) 库
const { GoogleGenerativeAI } = require('@google/generative-ai');

// 从环境变量中获取 Gemini API 密钥
const geminiApiKey = 't';//process.env.GEMINI_API_KEY;

// 检查 API 密钥是否已设置
if (!geminiApiKey) {
    console.error(JSON.stringify({ error: "GEMINI_API_KEY 环境变量未设置。" }));
    process.exit(1);
}
// 初始化 Gemini API 客户端
const genAI = new GoogleGenerativeAI(geminiApiKey);
async function generateText(prompt) {
    const model = genAI.getGenerativeModel({ model: "gemini-1.5-pro-latest" });  // 或者 "gemini-pro"
    try {
        const result = await model.generateContent(prompt); // 直接传递 prompt
        const response = await result.response;
        const text = response.text();
        return { answer: text };
    } catch (error) {
        console.error("Gemini API 调用出错:", error);
        return { error: error.message }; // 返回错误信息
    }
}
async function main() {
    // 获取命令行参数
    const args = process.argv.slice(2);
    if (args.length === 0) {
        console.error(JSON.stringify({ error: "No question provided." }));
        process.exit(1);
    }
    const userQuestion = args[0];
    // 调用 generateText 函数
    const result = await generateText(userQuestion);
    // 将结果转换为 JSON 字符串并打印到控制台
    console.log(JSON.stringify(result));
}
main();
