/*
 * Copyright 2013 Rui Araújo, Luís Fonseca
 *
 * This file is part of Router Keygen.
 *
 * Router Keygen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Router Keygen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Router Keygen.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "WirelessMatcher.h"
#include "config/AliceMagicInfo.h"
#include "config/TeleTuMagicInfo.h"
#include "config/AliceConfigParser.h"
#include "config/TeleTuConfigParser.h"
#include "config/OTEHuaweiConfigParser.h"
#include "algorithms/Keygen.h"
#include "algorithms/TecomKeygen.h"
#include "algorithms/TeleTuKeygen.h"
#include "algorithms/ThomsonKeygen.h"
#include "algorithms/VerizonKeygen.h"
#include "algorithms/InfostradaKeygen.h"
#include "algorithms/EircomKeygen.h"
#include "algorithms/SkyV1Keygen.h"
#include "algorithms/Wlan2Keygen.h"
#include "algorithms/ComtrendKeygen.h"
#include "algorithms/ZyxelKeygen.h"
#include "algorithms/Wlan6Keygen.h"
#include "algorithms/DiscusKeygen.h"
#include "algorithms/DlinkKeygen.h"
#include "algorithms/PirelliKeygen.h"
#include "algorithms/TelseyKeygen.h"
#include "algorithms/OnoKeygen.h"
#include "algorithms/HuaweiKeygen.h"
#include "algorithms/AliceItalyKeygen.h"
#include "algorithms/AliceGermanyKeygen.h"
#include "algorithms/ConnKeygen.h"
#include "algorithms/AxtelKeygen.h"
#include "algorithms/AndaredKeygen.h"
#include "algorithms/MegaredKeygen.h"
#include "algorithms/MaxcomKeygen.h"
#include "algorithms/InterCableKeygen.h"
#include "algorithms/OteKeygen.h"
#include "algorithms/OteBAUDKeygen.h"
#include "algorithms/OteHuaweiKeygen.h"
#include "algorithms/PBSKeygen.h"
#include "algorithms/PtvKeygen.h"
#include "algorithms/ArcadyanKeygen.h"
#include "algorithms/CabovisaoSagemKeygen.h"
#include "algorithms/Speedport500Keygen.h"
#include "algorithms/WifimediaRKeygen.h"
#include "algorithms/BelkinKeygen.h"
#include "algorithms/TplinkKeygen.h"
#include "algorithms/ArnetPirelliKeygen.h"
#include "algorithms/SitecomKeygen.h"
#include "algorithms/HG824xKeygen.h"
#include "algorithms/SitecomWLR400xKeygen.h"
#include "algorithms/SitecomWLR2100Keygen.h"
#include <QRegExp>

WirelessMatcher::WirelessMatcher() {
    supportedAlice = AliceConfigParser::readFile(":/alice.txt");
    supportedTeletu = TeleTuConfigParser::readFile(":/tele2.txt");
    supportedOTE = OTEHuaweiConfigParser::readFile(":/ote_huawei.txt");
}

WirelessMatcher::~WirelessMatcher() {
    QList<QString> keys = supportedAlice->keys();
    for (int i = 0; i < keys.size(); ++i) {
        QVector<AliceMagicInfo *> * supported = supportedAlice->value(
                keys.at(i));
        for (int j = 0; j < supported->size(); ++j)
            delete supported->at(j);
        delete supported;
    }
    supportedAlice->clear();
    delete supportedAlice;
    keys = supportedTeletu->keys();
    for (int i = 0; i < keys.size(); ++i) {
        QVector<TeleTuMagicInfo *> * supported = supportedTeletu->value(
                keys.at(i));
        for (int j = 0; j < supported->size(); ++j)
            delete supported->at(j);
        delete supported;
    }
    supportedTeletu->clear();
    delete supportedTeletu;
    delete supportedOTE;
}

QVector<Keygen *> * WirelessMatcher::getKeygens(QString ssid, QString mac) {
    QVector<Keygen *> * keygens = new QVector<Keygen*>;

    QVector<AliceMagicInfo *> * supported = supportedAlice->value(
            ssid.mid(6,3));
    if (supported != NULL && supported->size() > 0) {
        QString macProcessed = mac.replace(":", "").replace("-", "").toUpper();
        if (macProcessed.length() < 6 || macProcessed.left(6) != supported->at(0)->mac) {
            macProcessed = supported->at(0)->mac;
        } else {
            macProcessed = mac;
        }
        keygens->append(new AliceItalyKeygen(ssid, macProcessed, supported));
    }

    keygens->append(new AliceGermanyKeygen(ssid, mac));
    keygens->append(new AndaredKeygen(ssid, mac));
    keygens->append(new ArcadyanKeygen(ssid, mac));
    keygens->append(new ArnetPirelliKeygen(ssid, mac));
    keygens->append(new AxtelKeygen(ssid, mac));
    keygens->append(new CabovisaoSagemKeygen(ssid, mac));
    keygens->append(new ConnKeygen(ssid, mac));
    keygens->append(new DiscusKeygen(ssid, mac));
    keygens->append(new DlinkKeygen(ssid, mac));
    keygens->append(new EircomKeygen(ssid, mac));
    keygens->append(new HuaweiKeygen(ssid, mac));
    keygens->append(new HG824xKeygen(ssid, mac));
    keygens->append(new InfostradaKeygen(ssid, mac));
    keygens->append(new InterCableKeygen(ssid, mac));
    keygens->append(new MaxcomKeygen(ssid, mac));
    keygens->append(new MegaredKeygen(ssid, mac));
    keygens->append(new OnoKeygen(ssid, mac));
    keygens->append(new OteBAUDKeygen(ssid, mac));
    keygens->append(new OteKeygen(ssid, mac));
    QString filteredMac = mac.replace(":", "");
    int target = filteredMac.mid(8).toInt(NULL, 16);
    if (filteredMac.length() == 12
        && target > (OteHuaweiKeygen::MAGIC_NUMBER - supportedOTE->length()))
        keygens->append(new OteHuaweiKeygen(ssid, mac,
                                   supportedOTE->at(OteHuaweiKeygen::MAGIC_NUMBER - target)));

    keygens->append(new PBSKeygen(ssid, mac));
    keygens->append(new BelkinKeygen(ssid, mac));
    keygens->append(new PirelliKeygen(ssid, mac));
    keygens->append(new PtvKeygen(ssid, mac));
    keygens->append(new SitecomKeygen(ssid, mac));
    keygens->append(new SitecomWLR400xKeygen(ssid, mac));
    keygens->append(new SitecomWLR2100Keygen(ssid, mac));
    keygens->append(new SkyV1Keygen(ssid, mac));
    keygens->append(new Speedport500Keygen(ssid, mac));
    keygens->append(new TecomKeygen(ssid, mac));

    if (filteredMac.length() != 12 &&
        (ssid.count(QRegExp("^TeleTu_[0-9a-fA-F]{12}$")) == 1)){
        mac = filteredMac = ssid.mid(7);
    }
    if (filteredMac.length() == 12) {
        QVector<TeleTuMagicInfo *> *  supported = supportedTeletu
                                                  ->value(filteredMac.left(6));
        if (supported != NULL && supported->size() > 0) {
            int macIntValue = filteredMac.mid(6).toInt(NULL,16);
            for (int i = 0; i < supported->size(); ++i ) {
                if (macIntValue >= supported->at(i)->range[0]
                    && macIntValue <= supported->at(i)->range[1]) {
                    keygens->append(new TeleTuKeygen(ssid, mac,
                                            supported->at(i)));
                }
            }
        }
    }

    keygens->append(new TelseyKeygen(ssid, mac));
    keygens->append(new ThomsonKeygen(ssid, mac));
    keygens->append(new TplinkKeygen(ssid, mac));
    keygens->append(new VerizonKeygen(ssid, mac));
    keygens->append(new WifimediaRKeygen(ssid, mac));
    keygens->append(new Wlan2Keygen(ssid, mac));
    keygens->append(new Wlan6Keygen(ssid, mac));
    keygens->append(new ZyxelKeygen(ssid, mac));
    keygens->append(new ComtrendKeygen(ssid, mac));

    return keygens;
}
