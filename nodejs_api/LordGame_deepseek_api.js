import OpenAI from "openai";

// 配置常量
const DEEPSEEK_API_BASE_URL = 'https://api.deepseek.com';
const DEEPSEEK_API_KEY = 't'; // 替换成你的 API Key
const DEEPSEEK_MODEL_NAME = "deepseek-chat";
const FALLBACK_STRATEGY = []; // Default to not playing if API fails (empty array).  Adjust based on your game's requirements.

const openai = new OpenAI({
    baseURL: DEEPSEEK_API_BASE_URL,
    apiKey: DEEPSEEK_API_KEY
});

// 卡牌枚举值到字符串的映射
const cardPointToString = {
    3: "Card_3",
    4: "Card_4",
    5: "Card_5",
    6: "Card_6",
    7: "Card_7",
    8: "Card_8",
    9: "Card_9",
    10: "Card_10",
    "J": "Card_J",
    "Q": "Card_Q",
    "K": "Card_K",
    "A": "Card_A",
    2: "Card_2",
    "SJ": "Card_SJ",
    "BJ": "Card_BJ"
};

// 字符串到卡牌枚举值的映射 (反向映射)
const stringToCardPoint = {
    "Card_3": 3,
    "Card_4": 4,
    "Card_5": 5,
    "Card_6": 6,
    "Card_7": 7,
    "Card_8": 8,
    "Card_9": 9,
    "Card_10": 10,
    "Card_J": "J",
    "Card_Q": "Q",
    "Card_K": "K",
    "Card_A": "A",
    "Card_2": 2,
    "Card_SJ": "SJ",
    "Card_BJ": "BJ"
};



async function main(requestData) {
    try {
        // 构建 DeepSeek API 的 prompt，将 requestData 包含在内
        const userQuestion = `根据以下信息判断是否应该出牌，如果应该出牌，请给出要出的牌的数组，如果不应该出牌，请返回一个空数组。请直接返回 JSON 格式的数组，不要添加任何其他解释。
            我的角色：${requestData.my_role}
            出牌玩家角色：${requestData.pen_player_role}
            出牌玩家出的牌：${requestData.pen_cards_value}
            我的手牌：${requestData.my_cards_value}
            是否有更大的牌：${requestData.has_greater_cards}
            请直接返回 JSON 格式的数组，例如：["Card_3", "Card_4"] 或 []。`;

        const completion = await openai.chat.completions.create({
            messages: [{ role: "system", content: "你是一个斗地主AI助手，根据自己和出牌玩家的身份、手牌、手牌数量信息判断是否出牌，并给出要出的牌的数组。只返回 JSON 格式的数组。" },
            { role: "user", content: userQuestion }],
            model: "deepseek- chat",
        });

        const answer = completion.choices[0].message.content;

        let cardsToPlay = []; // 默认值：空数组 (不出牌)
        try {
            cardsToPlay = JSON.parse(answer); // 尝试解析答案为 JSON 数组

            // 验证数组中的元素是否为有效的 CardPoint 字符串
            if (!Array.isArray(cardsToPlay) || !cardsToPlay.every(card => stringToCardPoint[card])) {
                console.warn("无效的 CardPoint 数组:", answer);
                cardsToPlay = FALLBACK_STRATEGY; // 使用默认策略：不出牌
            }
        } catch (e) {
            console.warn("无法解析模型 API 的响应:", answer);
            cardsToPlay = FALLBACK_STRATEGY; // 使用默认策略：不出牌
        }

        console.log(JSON.stringify(cardsToPlay)); // 返回 JSON 格式的结果

    } catch (error) {
        console.error("DeepSeek API 调用失败:", error);
        console.log(JSON.stringify(FALLBACK_STRATEGY)); // 返回默认策略：不出牌
    }
}

// 获取命令行参数 (JSON 字符串)
const args = process.argv.slice(2);
let requestData = {};
try {
    requestData = JSON.parse(args[0] || "{}"); // 尝试解析 JSON 数据
} catch (e) {
    console.error("无效的 JSON 数据:", args[0]);
    console.log(JSON.stringify(FALLBACK_STRATEGY)); // 返回默认策略：不出牌
    process.exit(1); // 退出程序
}

main(requestData);
