#include <iostream>
#include <string>

using namespace std;

int main(){
    string cipher = "BEEAKFYDJXUQYHYJIQRYHTYJIQFBQDUYJIIKFUHCQD";
    string plain;
    int len = cipher.size();
    for (int dis = 0; dis < 26; dis++){
        plain.clear();
        for (int i = 0; i < len; i++){
            plain += 'A' + (cipher[i] - 'A' + dis) % 26;
        }
        cout << "dis = " << dis << ", plaintext is " << plain << endl;
    }
    return 0;
    /******************************* Output **********************************/
    /*  dis = 0, plaintext is BEEAKFYDJXUQYHYJIQRYHTYJIQFBQDUYJIIKFUHCQD
        dis = 1, plaintext is CFFBLGZEKYVRZIZKJRSZIUZKJRGCREVZKJJLGVIDRE
        dis = 2, plaintext is DGGCMHAFLZWSAJALKSTAJVALKSHDSFWALKKMHWJESF
        dis = 3, plaintext is EHHDNIBGMAXTBKBMLTUBKWBMLTIETGXBMLLNIXKFTG
        dis = 4, plaintext is FIIEOJCHNBYUCLCNMUVCLXCNMUJFUHYCNMMOJYLGUH
        dis = 5, plaintext is GJJFPKDIOCZVDMDONVWDMYDONVKGVIZDONNPKZMHVI
        dis = 6, plaintext is HKKGQLEJPDAWENEPOWXENZEPOWLHWJAEPOOQLANIWJ
        dis = 7, plaintext is ILLHRMFKQEBXFOFQPXYFOAFQPXMIXKBFQPPRMBOJXK
        dis = 8, plaintext is JMMISNGLRFCYGPGRQYZGPBGRQYNJYLCGRQQSNCPKYL
        dis = 9, plaintext is KNNJTOHMSGDZHQHSRZAHQCHSRZOKZMDHSRRTODQLZM
        dis = 10, plaintext is LOOKUPINTHEAIRITSABIRDITSAPLANEITSSUPERMAN
        dis = 11, plaintext is MPPLVQJOUIFBJSJUTBCJSEJUTBQMBOFJUTTVQFSNBO
        dis = 12, plaintext is NQQMWRKPVJGCKTKVUCDKTFKVUCRNCPGKVUUWRGTOCP
        dis = 13, plaintext is ORRNXSLQWKHDLULWVDELUGLWVDSODQHLWVVXSHUPDQ
        dis = 14, plaintext is PSSOYTMRXLIEMVMXWEFMVHMXWETPERIMXWWYTIVQER
        dis = 15, plaintext is QTTPZUNSYMJFNWNYXFGNWINYXFUQFSJNYXXZUJWRFS
        dis = 16, plaintext is RUUQAVOTZNKGOXOZYGHOXJOZYGVRGTKOZYYAVKXSGT
        dis = 17, plaintext is SVVRBWPUAOLHPYPAZHIPYKPAZHWSHULPAZZBWLYTHU
        dis = 18, plaintext is TWWSCXQVBPMIQZQBAIJQZLQBAIXTIVMQBAACXMZUIV
        dis = 19, plaintext is UXXTDYRWCQNJRARCBJKRAMRCBJYUJWNRCBBDYNAVJW
        dis = 20, plaintext is VYYUEZSXDROKSBSDCKLSBNSDCKZVKXOSDCCEZOBWKX
        dis = 21, plaintext is WZZVFATYESPLTCTEDLMTCOTEDLAWLYPTEDDFAPCXLY
        dis = 22, plaintext is XAAWGBUZFTQMUDUFEMNUDPUFEMBXMZQUFEEGBQDYMZ
        dis = 23, plaintext is YBBXHCVAGURNVEVGFNOVEQVGFNCYNARVGFFHCREZNA
        dis = 24, plaintext is ZCCYIDWBHVSOWFWHGOPWFRWHGODZOBSWHGGIDSFAOB
        dis = 25, plaintext is ADDZJEXCIWTPXGXIHPQXGSXIHPEAPCTXIHHJETGBPC */

    // The plaintext is found when dis = 10:
    // "Look up in the air. It’s a bird. It’s a plane. It’s superman."
}