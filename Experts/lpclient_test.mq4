//+------------------------------------------------------------------+
//|                                                lpclient_test.mq4 |
//|                     Copyright 2019, sebastian.wilwerth@gmail.com |
//|                 https://github.com/swilwerth/mql-lpclient-latest |
//+------------------------------------------------------------------+
#property copyright "Copyright 2019, sebastian.wilwerth@gmail.com"
#property link      "https://github.com/swilwerth/mql-lpclient-latest"
#property version   "1.0"
#property strict

#include <LPClient/LPC.mqh>

input string wkr0 = "tcp://localhost:5555"; //Echo Worker A address
input string wkr1 = "tcp://localhost:5566"; //Echo Worker B address

LPClient* workerA;
LPClient* workerB;

const string worker_a_name()   { return "WorkerA"; }
const string worker_a_addr()   { return wkr0; }
const string worker_b_name()   { return "WorkerB"; }
const string worker_b_addr()   { return wkr1; }
const string nullworker_name() { return "Offline Worker"; }
const string nullworker_addr() { return "tcp://invalidhost:5555"; }

void halt(string reason) {
    printf(reason);
    OnDeinit(ERR_INTERNAL_ERROR);
    while(true)
        Sleep(1);
}

void assert(bool res, string reason) {
    if(!res)
        halt(reason);
}

const string rPay()    { return "#this is a R script"; }
const string jsonPay() { return "{ \"some_json_log\": { \"SYMBOL\": \"EURUSD\", \"MAGIC\": \"42\", \"etc ...\":\"etc\" }}"; }
const string pangrams(int id, int &max) {
    string pangrams[] =  {
        "The quick brown fox jumps over the lazy dog",
        "Falsches Üben von Xylophonmusik quält jeden größeren Zwerg",
        "Høj bly gom vandt fræk sexquiz på wc",
        "El veloz murciélago hindú comía feliz cardillo y kiwi. La cigüeña tocaba el saxofón detrás del palenque de paja",
        "Ταχίστη αλώπηξ βαφής ψημένη γη, δρασκελίζει υπέρ νωθρού κυνός",
        "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ",
        "Hyvän lorun sangen pieneksi hyödyksi jäi suomen kirjaimet",
        "千字文（簡化字版）敕员外散骑侍郎周兴嗣 次韵 天地玄黄，宇宙洪荒。日月盈昃，辰宿列张。寒来暑往，秋收冬藏。闰余成岁，律吕调阳。云腾致雨，露结为霜。金生丽水，玉出昆冈。剑号巨阙，珠称夜光。果珍李柰，菜重芥姜。海咸河淡，鳞潜羽翔。龙师火帝，鸟官人皇。始制文字，乃服衣裳。推位让国，有虞陶唐。吊民伐罪，周发殷汤。坐朝问道，垂拱平章。爱育黎首，臣伏戎羌。遐迩壹体，率宾归王。鸣凤在树，白驹食场。化被草木，赖及万方。盖此身发，四大五常。恭惟鞠养，岂敢毁伤。女慕贞洁，男效才良。知过必改，得能莫忘。罔谈彼短，靡恃己长。信使可覆，器欲难量。千字文 简化字版 第1页 墨悲丝染，诗赞羔羊。景行维贤，克念作圣。德建名立，形端表正。空谷传声，虚堂习听。祸因恶积，福缘善庆。尺璧非宝，寸阴是竞。资父事君，曰严与敬。孝当竭力，忠则尽命。临深履薄，夙兴温凊。似兰斯馨，如松之盛。川流不息，渊澄取映。容止若思，言辞安定。笃初诚美，慎终宜令。荣业所基，藉甚无竟。学优登仕，摄职从政。存以甘棠，去而益咏。乐殊贵贱，礼别尊卑。上和下睦，夫唱妇随。外受傅训，入奉母仪。诸姑伯叔，犹子比儿。孔怀兄弟，同气连枝。交友投分，切磨箴规。仁慈隐恻，造次弗离。节义廉退，颠沛匪亏。性静情逸，心动神疲。守真志满，逐物意移。坚持雅操，好爵自縻。都邑华夏，东西二京。背邙面洛，浮渭据泾。宫殿盘郁，楼观飞惊。千字文 简化字版 第2页 图写禽兽，画彩仙灵。丙舍傍启，甲帐对楹。肆筵设席，鼓瑟吹笙。升阶纳陛，弁转疑星。右通广内，左达承明。既集坟典，亦聚群英。杜稿钟隶，漆书壁经。府罗将相，路侠槐卿。户封八县，家给千兵。高冠陪辇，驱毂振缨。世禄侈富，车驾肥轻。策功茂实，勒碑刻铭。磻溪伊尹，佐时阿衡。奄宅曲阜，微旦孰营。桓公匡合，济弱扶倾。绮回汉惠，说感武丁。俊乂密勿，多士实宁。晋楚更霸，赵魏困横。假途灭虢，践土会盟。何遵约法，韩弊烦刑。起翦颇牧，用军最精。宣威沙漠，驰誉丹青。九州禹迹，百郡秦并。岳宗恒岱，禅主云亭。雁门紫塞，鸡田赤城。昆池碣石，巨野洞庭。旷远绵邈，岩岫杳冥。治本于农，务兹稼穑。俶载南亩，我艺黍稷。税熟贡新，劝赏黜陟。千字文 简化字版第3页 孟轲敦素，史鱼秉直。庶几中庸，劳谦谨敕。聆音察理，鉴貌辨色。贻厥嘉猷，勉其祗植。省躬讥诫，宠增抗极。殆辱近耻，林皋幸即。两疏见机，解组谁逼。索居闲处，沉默寂寥。求古寻论，散虑逍遥。欣奏累遣，戚谢欢招。渠荷的历，园莽抽条。枇杷晚翠，梧桐早凋。陈根委翳，落叶飘摇。游鹍独运，凌摩绛霄。耽读玩市，寓目囊箱。易輶攸畏，属耳垣墙。具膳餐饭，适口充肠。饱饫烹宰，饥厌糟糠。亲戚故旧，老少异粮。妾御绩纺，侍巾帷房。纨扇圆洁，银烛炜煌。昼眠夕寐，蓝笋象床。弦歌酒宴，接杯举觞。矫手顿足，悦豫且康。嫡后嗣续，祭祀烝尝。稽颡再拜，悚惧恐惶。笺牒简要，顾答审详。骸垢想浴，执热愿凉。驴骡犊特，骇跃超骧。诛斩贼盗，捕获叛亡。千字文 简化字版第4页 布射辽丸，嵇琴阮啸。恬笔伦纸，钧巧任钓。释纷利俗，并皆佳妙。毛施淑姿，工颦妍笑。年矢每催，曦晖朗曜。璇玑悬斡，晦魄环照。指薪修祜，永绥吉劭。矩步引领，俯仰廊庙。束带矜庄，徘徊瞻眺。孤陋寡闻，愚蒙等诮。谓语助者，焉哉乎也。李祥工作室‧校正整理‧二○一一年八月二十七日千字文 简化字版232第5页千字文（簡化字版）敕员外散骑侍郎周兴嗣 次韵 天地玄黄，宇宙洪荒。日月盈昃，辰宿列张。寒来暑往，秋收冬藏。闰余成岁，律吕调阳。云腾致雨，露结为霜。金生丽水，玉出昆冈。剑号巨阙，珠称夜光。果珍李柰，菜重芥姜。海咸河淡，鳞潜羽翔。龙师火帝，鸟官人皇。始制文字，乃服衣裳。推位让国，有虞陶唐。吊民伐罪，周发殷汤。坐朝问道，垂拱平章。爱育黎首，臣伏戎羌。遐迩壹体，率宾归王。鸣凤在树，白驹食场。化被草木，赖及万方。盖此身发，四大五常。恭惟鞠养，岂敢毁伤。女慕贞洁，男效才良。知过必改，得能莫忘。罔谈彼短，靡恃己长。信使可覆，器欲难量。千字文 简化字版 第1页 墨悲丝染，诗赞羔羊。景行维贤，克念作圣。德建名立，形端表正。空谷传声，虚堂习听。祸因恶积，福缘善庆。尺璧非宝，寸阴是竞。资父事君，曰严与敬。孝当竭力，忠则尽命。临深履薄，夙兴温凊。似兰斯馨，如松之盛。川流不息，渊澄取映。容止若思，言辞安定。笃初诚美，慎终宜令。荣业所基，藉甚无竟。学优登仕，摄职从政。存以甘棠，去而益咏。乐殊贵贱，礼别尊卑。上和下睦，夫唱妇随。外受傅训，入奉母仪。诸姑伯叔，犹子比儿。孔怀兄弟，同气连枝。交友投分，切磨箴规。仁慈隐恻，造次弗离。节义廉退，颠沛匪亏。性静情逸，心动神疲。守真志满，逐物意移。坚持雅操，好爵自縻。都邑华夏，东西二京。背邙面洛，浮渭据泾。宫殿盘郁，楼观飞惊。千字文 简化字版 第2页 图写禽兽，画彩仙灵。丙舍傍启，甲帐对楹。肆筵设席，鼓瑟吹笙。升阶纳陛，弁转疑星。右通广内，左达承明。既集坟典，亦聚群英。杜稿钟隶，漆书壁经。府罗将相，路侠槐卿。户封八县，家给千兵。高冠陪辇，驱毂振缨。世禄侈富，车驾肥轻。策功茂实，勒碑刻铭。磻溪伊尹，佐时阿衡。奄宅曲阜，微旦孰营。桓公匡合，济弱扶倾。绮回汉惠，说感武丁。俊乂密勿，多士实宁。晋楚更霸，赵魏困横。假途灭虢，践土会盟。何遵约法，韩弊烦刑。起翦颇牧，用军最精。宣威沙漠，驰誉丹青。九州禹迹，百郡秦并。岳宗恒岱，禅主云亭。雁门紫塞，鸡田赤城。昆池碣石，巨野洞庭。旷远绵邈，岩岫杳冥。治本于农，务兹稼穑。俶载南亩，我艺黍稷。税熟贡新，劝赏黜陟。千字文 简化字版第3页 孟轲敦素，史鱼秉直。庶几中庸，劳谦谨敕。聆音察理，鉴貌辨色。贻厥嘉猷，勉其祗植。省躬讥诫，宠增抗极。殆辱近耻，林皋幸即。两疏见机，解组谁逼。索居闲处，沉默寂寥。求古寻论，散虑逍遥。欣奏累遣，戚谢欢招。渠荷的历，园莽抽条。枇杷晚翠，梧桐早凋。陈根委翳，落叶飘摇。游鹍独运，凌摩绛霄。耽读玩市，寓目囊箱。易輶攸畏，属耳垣墙。具膳餐饭，适口充肠。饱饫烹宰，饥厌糟糠。亲戚故旧，老少异粮。妾御绩纺，侍巾帷房。纨扇圆洁，银烛炜煌。昼眠夕寐，蓝笋象床。弦歌酒宴，接杯举觞。矫手顿足，悦豫且康。嫡后嗣续，祭祀烝尝。稽颡再拜，悚惧恐惶。笺牒简要，顾答审详。骸垢想浴，执热愿凉。驴骡犊特，骇跃超骧。诛斩贼盗，捕获叛亡。千字文 简化字版第4页 布射辽丸，嵇琴阮啸。恬笔伦纸，钧巧任钓。释纷利俗，并皆佳妙。毛施淑姿，工颦妍笑。年矢每催，曦晖朗曜。璇玑悬斡，晦魄环照。指薪修祜，永绥吉劭。矩步引领，俯仰廊庙。束带矜庄，徘徊瞻眺。孤陋寡闻，愚蒙等诮。谓语助者，焉哉乎也。李祥工作室‧校正整理‧二○一一年八月二十七日千字文 简化字版232第5页",
        "ऋषियों को सताने वाले दुष्ट राक्षसों के राजा रावण का सर्वनाश करने वाले विष्णुवतार भगवान श्रीराम, अयोध्या के महाराज दशरथ के बड़े सपुत्र थे।",
        "עטלף אבק נס דרך מזגן שהתפוצץ כי חם"
    };
    max = ArraySize(pangrams);
    return pangrams[id];
}

void setupWorkers() {
    workerA = new LPClient(worker_a_name(), worker_a_addr());
    workerB = new LPClient(worker_b_name(), worker_b_addr());
}

void int_echo_t() {
    for(int i = 0xFFFFFF-100; i < 0xFFFFFF; i++)
        assert(i == int_echo(i), "Error at int_echo() test");
}

class MarshallingTests: public STRMarshaller {
    public:
    void char_echo_t() {
        char hello[];
        int strlen = toCharArray("char* string echo test", hello);
        char creply[1024] = {0};
        char_echo(hello, creply, strlen);
        assert(ArrayCompare(hello, creply, 0, 0, strlen) == 0, "Error in char_echo() test");
        string reply; fromCharArray(creply, strlen, reply);
        printf("%s (reply len: %d) ", reply, strlen);
    }

    void pangram_echo_t() {
        int max = 0xFFFFF;
        for(int i=0; i < max; i++) {
            char pangram_reply[] = {0};
            ArrayResize(pangram_reply, StringLen(pangrams(i, max)));
            char cpan[];
            int size = toCharArray(pangrams(i, max), cpan);
            char_echo(cpan, pangram_reply, size);
            assert(ArrayCompare(cpan, pangram_reply, 0, 0, size) == 0, "Error in pangram_echo() test");
        }
    }

    void api_non_init_worker_t() {
        int aux;
        char msg[];
        int size = toCharArray("A dummy message", msg);
        char buf[4096];
        for(int i = 0; i < LPC_MAX_WORKERS; i++) {
            if(i > 1) {
                aux = worker_echo(i, msg, size);
                assert(aux == -1, "worker_echo() should return -1 here");

                aux = worker_send(i, msg, size);
                assert(aux == 0, "worker_send() should return 0 here");

                aux = worker_getreplysize(i);
                assert(aux == 0, "worker_getreplysize() should return 0 here");

                worker_getreply(i, buf);
                aux = 2;
                string sbuf; fromCharArray(buf, aux, sbuf);
                assert(StringLen(sbuf) == 0, "worker_getreply() should return a 0 length string");

                worker_getLastError(i, aux);
                assert(aux == LPC_ERR_INVALIDHANDLE, "worker_getLastError should return an invalid handle code");

                worker_getLErrContext(i, buf, aux);
                assert(aux == -1, "worker_getLErrContext() should not return anything");

                worker_setRequestTimeout(i,REQUEST_TIMEOUT);
                worker_setRequestRetries(i,REQUEST_RETRIES);

                worker_getRequestTimeout(i,aux);
                assert(aux == LPC_ERR_INVALIDHANDLE, "worker_getRequestTimeout() should return an invalid handle code");

                worker_getRequestRetries(i,aux);
                assert(aux == LPC_ERR_INVALIDHANDLE, "worker_getRequestRetries() should return an invalid handle code");

                int len = toCharArray(nullworker_addr(), buf);
                worker_setaddr(i, buf, len);
                worker_getaddr(i, buf, aux);
                assert(aux == -1, "worker_getaddr() should not return anything");

                len = toCharArray(nullworker_name(), buf);
                worker_setname(i, buf, len);
                worker_getname(i, buf, aux);
                assert(aux == -1, "worker_getname() should not return anything");
            }
        }
    }

    MarshallingTests() { defaultCP = CP_THREAD_ACP; }
};

void api_pangram_t() {
    int max = 0xFFFFF;
    int aux = 0;
    string reply;
    for(int i=0; i < max; i++) {
        bool notsucess = true;
        while(notsucess) {
            workerA.sendTX(pangrams(i,max));
            if(workerA.getLastReplySize() > 0)
                notsucess = false;
        }
        reply = workerA.getLastReply();
        if(reply != pangrams(i, max))
            printf("Error in api_pangram_t() test, pangram corrupted: %s", pangrams(i, max));
    }
}

void api_online_t() {
    int aux = 0;
    string reply = "";
    string msg = "A dummy echo test message";
    printf("Testing worker connectivity ...");
    workerA.setRequestTimeout(REQUEST_TIMEOUT * 4);
    reply = workerA.sendTX(msg);
    assert(reply == msg, "Echo worker needs to be online to perform online tests");
    aux = workerA.getLastError();
    assert(aux == 0, "Echo worker needs to be online to perform online tests");
    printf("Done!");
    printf("Pangram echo tests ... ");
    api_pangram_t();
    printf("Done!");
}

void api_badhost_t() {
   delete(workerA);
   delete(workerB);
   string msg = "A dummy echo test message";
   workerA = new LPClient(nullworker_name(),nullworker_addr());
   workerA.sendTX(msg);
   int aux = workerA.getLastError();
   assert(aux == LPC_ERR_REXCEED, "Worker should exit with LPC_ERR_REXCEED error code");
   delete(workerA);
}


void std_tests() {
    MarshallingTests mt();
    printf("Testing integer echo ... ");
    int_echo_t();
    printf("Done!");
    printf("Testing standard char string echo ... ");
    mt.char_echo_t();
    printf("Done!");
    printf("Testing pangrams string echo ... ");
    mt.pangram_echo_t();
    printf("Done!");
}

void api_non_transactional_t() {
    printf("Testing worker_getaddr() ... ");
    string addr = workerA.getAddr();
    assert(addr == worker_a_addr(), "worker_getaddr() fails");
    printf("Done!");

    printf("Testing worker_getname() ... ");
    string name = workerA.getName();
    assert(name == worker_a_name(), "worker_getname() fails");
    printf("Done!");

    printf("Testing worker_getRequestTimeout() ... ");
    int aux = workerA.getRequestTimeout();
    assert(aux == REQUEST_TIMEOUT, "worker_getRequestTimeout() default value missing");
    printf("Done!");

    printf("Testing worker_getRequestRetries() ... ");
    aux = workerA.getRequestRetries();
    assert(aux == REQUEST_RETRIES, "worker_getRequestRetries() default value missing");
    printf("Done!");

    printf("Testing worker_setaddr() ... ");
    workerA.setAddr(nullworker_addr());
    addr = workerA.getAddr();
    assert(addr == nullworker_addr(), "worker_setaddr() fails");
    workerA.setAddr(worker_a_addr());
    printf("Done!");

    printf("Testing worker_setname() ... ");
    workerA.setName(nullworker_name());
    name = workerA.getName();
    assert(name == nullworker_name(), "worker_setname() fails");
    workerA.setName(worker_a_name());
    printf("Done!");

    printf("Testing worker_setRequestTimeout() ... ");
    aux = REQUEST_TIMEOUT * 4; workerA.setRequestTimeout(aux);
    assert(aux == REQUEST_TIMEOUT * 4, "worker_setRequestTimeout() setter error");
    aux = REQUEST_TIMEOUT; workerA.setRequestTimeout(aux);
    printf("Done!");

    printf("Testing worker_setRequestRetries() ... ");
    aux = REQUEST_RETRIES * 4; workerA.setRequestRetries(aux);
    assert(aux == REQUEST_RETRIES * 4, "worker_setRequestRetries() setter error");
    aux = REQUEST_RETRIES; workerA.setRequestRetries(aux);
    printf("Done!");
}

//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+

int OnInit()  {
    MarshallingTests mt();
    printf(":: Initializing API types offline tests");
    std_tests();
    printf(":: Initializing Workers");
    setupWorkers();
    printf(":: Testing API non transactional getters & setters");
    api_non_transactional_t();
    printf(":: Testing API non initialized workers call access restriction");
    mt.api_non_init_worker_t();
    printf(":: Going to online echo tests");
    api_online_t();
    printf(":: Failed connection test (this might take a while)");
    api_badhost_t();
    printf(":: Re-initializing Workers");
    setupWorkers();
    printf(":: Starting REQ/REP infinite loop");
    return(INIT_SUCCEEDED);
}

//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason) {
    delete(workerA);
    delete(workerB);
}

//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick() {
    printf("reply body: %s", workerA.sendTX(rPay()));
    printf(workerA.getLErrContext());
    printf("reply body: %s", workerB.sendTX(jsonPay()));
    printf(workerB.getLErrContext());
}
//+------------------------------------------------------------------+
