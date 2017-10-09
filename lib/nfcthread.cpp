#include "nfcthread.h"
#include "lib/httpclient.h"
#include "lib/writelcd.h"
#include <QDebug>

NfcThread::NfcThread()
{

}
void NfcThread::run(){

    const nfc_modulation nmMifare = {
        .nmt = NMT_ISO14443A,
        .nbr = NBR_106,
    };
    QString id,url,lcd,repeat = " ";;
    HttpClient http;
    int i;
    WriteLcd *wLcd = new WriteLcd();
    wLcd->clear();
    wLcd->write(0,0,"Attesa badge");
    while(1){
         nfc_init(&context);
         pnd = nfc_open(context, NULL);
         if (pnd == NULL) {
             qDebug() << "ERROR: %s. Unable to open NFC device.";

         }else{

             qDebug() << "Nfc aperto";

             if (nfc_initiator_init(pnd) < 0) {
                 qDebug() << "nfc_initiator_init";
                 nfc_close(pnd);
                 nfc_exit(context);

             }else{
                 qDebug() << "Nfc iniator";
                 if(nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) > 0){
                     id = "";
                     for(i = 0; i < nt.nti.nai.szUidLen;i++){

                         QString hex;
                         id += hex.sprintf("%02x",nt.nti.nai.abtUid[i]).toUpper();

                     }
                     qDebug() << "Leggo: " << id;
                     lcd = "C:"+id;
                     lcd = lcd+repeat.repeated(16 - lcd.length());
                     wLcd->write(0,0,lcd.toUtf8().data());
                     url = "/default/json/newcard/key/"+id;
                     http.Get(url);
                     while(!nfc_initiator_target_is_present(pnd,&nt)){
                         sleep(1);
                     }
                     nfc_close(pnd);
                     nfc_exit(context);

                 }
             }
         }
     }

}
