#include "lpclient.hpp"
#include <assert.h>
#include <cstring>

const string worker_a_name() {
    return "WorkerA";
}

const string worker_a_addr() {
    return "tcp://localhost:5555";
}

const string worker_b_name() {
    return "WorkerB";
}

const string worker_b_addr() {
    return "tcp://localhost:5566";
}

const string nullworker_name() {
    return "Offline Worker";
}

const string nullworker_addr() {
    return "tcp://invalidhost:5555";
}

const string rPay() {
    string payload = "#this is a R script";
    return payload;
}

const string jsonPay() {
    string payload = "{ \"some_json_log\": { \"SYMBOL\": \"EURUSD\", \"MAGIC\": \"42\", \"etc ...\":\"etc\" }}";
    return payload;
}

const string pangrams(int id, int &max) {
    max = 9;
    string pangrams[max] =  {
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
    return pangrams[id];
}

void setupWorkers() {
    widptr_t handle;

    worker_add(
        worker_a_name().c_str(),
        worker_a_name().size(),
        worker_a_addr().c_str(),
        worker_a_addr().size(),
        handle
    );
    assert(handle == 0 && "setupWorkers() handle == 0 expected here");

    worker_add(
        worker_b_name().c_str(),
        worker_b_name().size(),
        worker_b_addr().c_str(),
        worker_b_addr().size(),
        handle
    );

    assert(handle == 1 && "setupWorkers() handle == 1 expected here");
}

void int_echo_t() {
    for(int i = 0xFFFFFF-100; i < 0xFFFFFF; i++)
        assert(i == int_echo((int_t) i) && "Error at int_echo() test");
}

void wstring_echo_t() {
    wchar_t reply[1024];
    wchar_t source[] = L"wchar_t* string echo test (";
    size_t size = string_echo(source, reply);
    wprintf(reply);
    assert(wcscmp(source, reply) == 0 && "Error in wstring_echo() test");
    printf("reply len: %d) ", size);
}

void char_echo_t() {
    char hello[] = "char* string echo test";
    string hell = hello;
    int strlen = hell.size();
    char creply[1024] = {0};
    char_echo(hello, creply, strlen);
    assert(strcmp(hello, creply) == 0 && "Error in char_echo() test");
    printf("%s (reply len: %d) ", creply, strlen);
}

void pangram_echo_t() {
    int max = 0xFFFFF;
    for(int i=0; i < max; i++) {
        char pangram_reply[pangrams(i,max).size()] = {0};
        char_echo(pangrams(i,max).c_str(), pangram_reply, pangrams(i,max).size());
        assert(strcmp(pangrams(i,max).c_str(), pangram_reply) == 0 && "Error in pangram_echo() test");
    }
}

void std_tests() {
    printf("Testing integer echo ... ");
    int_echo_t();
    printf("Done!\n");
    printf("Testing wide char string echo ... ");
    wstring_echo_t();
    printf("Done!\n");
    printf("Testing standard char string echo ... ");
    char_echo_t();
    printf("Done!\n");
    printf("Testing pangrams string echo ... ");
    pangram_echo_t();
    printf("Done!\n");
}

void api_non_transactional_t() {
    char buf[4096];
    int_t len = 0;
    int_t aux = 0;

    printf("Testing worker_getaddr() ... ");
    worker_getaddr(0, buf, len);
    assert(strcmp(buf, worker_a_addr().c_str()) == 0 && "worker_getaddr() fails");
    printf("Done!\n");

    printf("Testing worker_getname() ... ");
    worker_getname(0, buf, len);
    assert(strcmp(buf, worker_a_name().c_str()) == 0 && "worker_getname() fails");
    printf("Done!\n");

    printf("Testing worker_getRequestTimeout() ... ");
    worker_getRequestTimeout(0, aux);
    assert(aux == REQUEST_TIMEOUT && "worker_getRequestTimeout() default value missing");
    printf("Done!\n");

    printf("Testing worker_getRequestRetries() ... ");
    worker_getRequestRetries(0, aux);
    assert(aux == REQUEST_RETRIES && "worker_getRequestRetries() default value missing");
    printf("Done!\n");

    printf("Testing worker_setaddr() ... ");
    worker_setaddr(0, nullworker_addr().c_str(), nullworker_addr().size());
    worker_getaddr(0, buf, len);
    assert(strcmp(buf, nullworker_addr().c_str()) == 0 && "worker_setaddr() fails");
    worker_setaddr(0, worker_a_addr().c_str(), worker_a_addr().size());
    printf("Done!\n");

    printf("Testing worker_setname() ... ");
    worker_setname(0, nullworker_name().c_str(), nullworker_name().size());
    worker_getname(0, buf, len);
    assert(strcmp(buf, nullworker_name().c_str()) == 0 && "worker_setname() fails");
    worker_setname(0, worker_a_name().c_str(), worker_a_name().size());
    printf("Done!\n");

    printf("Testing worker_setRequestTimeout() ... ");
    aux = REQUEST_TIMEOUT * 4; worker_setRequestTimeout(0, aux);
    assert(aux == REQUEST_TIMEOUT * 4 && "worker_setRequestTimeout() default value missing");
    aux = REQUEST_TIMEOUT; worker_setRequestTimeout(0, aux);
    printf("Done!\n");

    printf("Testing worker_setRequestRetries() ... ");
    aux = REQUEST_RETRIES * 4; worker_setRequestRetries(0, aux);
    assert(aux == REQUEST_RETRIES * 4 && "worker_setRequestRetries() default value missing");
    aux = REQUEST_RETRIES; worker_setRequestRetries(0, aux);
    printf("Done!\n");
}

void api_non_init_worker_t() {
    int_t aux;
    char msg[] = "A dummy message";
    char buf[4096];
    for(int_t i = 0; i < LPC_MAX_WORKERS; i++) {
        if(i > 1) {
            aux = worker_echo(i, msg, strlen(msg));
            assert(aux == -1 && "worker_echo() should return -1 here");

            aux = worker_send(i, msg, strlen(msg));
            assert(aux == 0 && "worker_send() should return 0 here");

            aux = worker_getreplysize(i);
            assert(aux == 0 && "worker_getreplysize() should return 0 here");

            worker_getreply(i, buf);
            assert(strlen(buf) == 0 && "worker_getreply() should return a 0 length string");

            worker_getLastError(i, aux);
            assert(aux == LPC_ERR_INVALIDHANDLE && "worker_getLastError should return an invalid handle code");

            worker_getLErrContext(i, buf, aux);
            assert(aux == -1 && "worker_getLErrContext() should not return anything");

            worker_setRequestTimeout(i,REQUEST_TIMEOUT);
            worker_setRequestRetries(i,REQUEST_RETRIES);

            worker_getRequestTimeout(i,aux);
            assert(aux == LPC_ERR_INVALIDHANDLE && "worker_getRequestTimeout() should return an invalid handle code");

            worker_getRequestRetries(i,aux);
            assert(aux == LPC_ERR_INVALIDHANDLE && "worker_getRequestRetries() should return an invalid handle code");

            worker_setaddr(i, nullworker_addr().c_str(), nullworker_addr().size());
            worker_getaddr(i, buf, aux);
            assert(aux == -1 && "worker_getaddr() should not return anything");

            worker_setname(i, nullworker_name().c_str(), nullworker_name().size());
            worker_getname(i, buf, aux);
            assert(aux == -1 && "worker_getname() should not return anything");
        }
    }
}

void api_pangram_t() {
    int max = 0xFFFFF;
    int_t aux = 0;
    char* reply;
    for(int i=0; i < max; i++) {
        char pangram_reply[pangrams(i,max).size()] = {0};
        bool notsucess = true;
        while(notsucess) {
            worker_send(0, pangrams(i,max).c_str(), pangrams(i,max).size());
            if(worker_getreplysize(0) > 0)
                notsucess = false;
        }
        aux = worker_getreplysize(0);
        reply = new char[aux];
        worker_getreply(0, reply);
        assert(strcmp(pangrams(i,max).c_str(), reply) == 0 && "Error in api_pangram_t() test");
        delete [] reply;
    }
}

void api_online_t() {
    int_t aux = 0;
    char msg[] = "A dummy echo test message";
    printf("Testing worker connectivity ...\n");
    worker_setRequestTimeout(0, REQUEST_TIMEOUT * 4);
    aux = worker_echo(0, msg, strlen(msg));
    assert(aux == strlen(msg) && "Echo reply mismatch");
    worker_getLastError(0,aux);
    assert(aux == 0 && "Echo worker needs to be online to perform online tests");
    printf("Done!\n");
    printf("Pangram echo tests ... \n");
    api_pangram_t();
    printf("Done!\n");
}

void api_badhost_t() {
    worker_deinit();
    widptr_t handle;
    char msg[] = "A dummy echo test message";
    worker_add(
        nullworker_name().c_str(),
        nullworker_name().size(),
        nullworker_addr().c_str(),
        nullworker_addr().size(),
        handle
    );
    assert(handle == 0 && "api_badhost_t() handle == 0 expected here");

    worker_send(0, msg, strlen(msg));
    int_t aux;
    worker_getLastError(0, aux);
    assert(aux == LPC_ERR_REXCEED && "Worker should exit with LPC_ERR_REXCEED error code");
    worker_deinit();
}

void OnInit() {
    printf(":: Initializing API types offline tests\n");
    std_tests();
    printf(":: Initializing Workers\n");
    setupWorkers();
    printf(":: Testing API non transactional getters & setters\n");
    api_non_transactional_t();
    printf(":: Testing API non initialized workers call access restriction\n");
    api_non_init_worker_t();
    printf(":: Going to online echo tests\n");
    api_online_t();
    printf(":: Failed connection test\n");
    api_badhost_t();
    printf(":: Re-initializing Workers\n");
    setupWorkers();
    printf(":: Starting REQ/REP infinite loop\n");
}

void OnTick() {
    string rpay = rPay();
    string jsonpay = jsonPay();
    worker_echo(0, rpay.c_str(), rpay.size());
    worker_echo(1, jsonpay.c_str(), jsonpay.size());
}

int main(int argc, char** argv) {
    printf(":: Init Sucess!\n");
    OnInit();
    while(true) {
        OnTick();
    }
    return EXIT_SUCCESS;
}
