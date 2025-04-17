import OpenAI from "openai";

const openai = new OpenAI({
    baseURL: 'https://api.deepseek.com',
    apiKey: 'sk-6bf9b7e6043f419e9f04de6b4ea65f4c' // 替换成你的 API Key
});

async function main(requestData) {  // 现在接收 requestData 作为参数
    try {
        // 构建 DeepSeek API 的 prompt，将 requestData 包含在内
        const userQuestion = `根据以下信息判断是否应该出牌（回答 true 或 false）：
            我的角色：${requestData.my_role}
            出牌玩家角色：${requestData.pen_player_role}
            出牌玩家出的牌：${requestData.pen_cards_value}
            我的手牌：${requestData.my_cards_value}
            是否有更大的牌：${requestData.has_greater_cards}
            请仅返回 true 或 false，不要添加任何其他解释。`;  // IMPORTANT: Instructions to only return true or false

        const completion = await openai.chat.completions.create({
            messages: [{ role: "system", content: "你是一个斗地主AI助手，需要判断是否出牌。只返回 true 或 false。" },  // System message is important
                       { role: "user", content: userQuestion }],
            model: "deepseek-chat",
            //max_tokens: 10 // 限制 token 数量，因为我们只需要 true/false
        });

        const answer = completion.choices[0].message.content;

        // 解析答案，提取 should_play 的值
        let shouldPlay = false; // 默认值
        if (answer.toLowerCase().trim() === "true") {
            shouldPlay = true;
        } else if (answer.toLowerCase().trim() === "false") {
            shouldPlay = false;
        } else {
            console.warn("无法解析模型 API 的响应:", answer);
            //Handle error and make the decision to play/not play based on a predefined strategy
            //Implement fallback logic here
        }

        // 返回 JSON 格式的结果
        console.log(JSON.stringify({ should_play: shouldPlay }));

    } catch (error) {
        console.error("Error calling DeepSeek API:", error);
        console.log(JSON.stringify({ error: error.message || "Unknown error" })); // 返回错误信息
    }
}

// 获取命令行参数 (JSON 字符串)
const args = process.argv.slice(2);
let requestData = {};
try {
    requestData = JSON.parse(args[0] || "{}"); // 尝试解析 JSON 数据
} catch (e) {
    console.error("Invalid JSON data:", args[0]);
    //Handle error - provide a default strategy or exit.
}

main(requestData);
