/*****************************************************************************
 * iso639.cpp - QStarDict, a StarDict clone written with using Qt            *
 * Copyright (C) 2008 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#include "iso639.h"

namespace QStarDict
{

QHash<QString, QString> ISO639::m_languages;

QStringList ISO639::languages()
{
    if (m_languages.isEmpty())
        init();
    return m_languages.keys();
}

QString ISO639::languageName(const QString &code)
{
    if (m_languages.isEmpty())
        init();
    if (m_languages.contains(code))
        return m_languages.value(code);
    else
        return code;
}

void ISO639::init()
{
    m_languages["abk"] = QObject::tr("Abkhazian");
    m_languages["ace"] = QObject::tr("Achinese");
    m_languages["ach"] = QObject::tr("Acoli");
    m_languages["ada"] = QObject::tr("Adangme");
    m_languages["aar"] = QObject::tr("Afar");
    m_languages["afh"] = QObject::tr("Afrihili");
    m_languages["afr"] = QObject::tr("Afrikaans");
    m_languages["afa"] = QObject::tr("Afro-Asiatic (Other)");
    m_languages["aka"] = QObject::tr("Akan");
    m_languages["akk"] = QObject::tr("Akkadian");
    m_languages["alb"] = QObject::tr("Albanian");
    m_languages["ale"] = QObject::tr("Aleut");
    m_languages["alg"] = QObject::tr("Algonquian languages");
    m_languages["tut"] = QObject::tr("Altaic (Other)");
    m_languages["amh"] = QObject::tr("Amharic");
    m_languages["apa"] = QObject::tr("Apache languages");
    m_languages["ara"] = QObject::tr("Arabic");
    m_languages["arc"] = QObject::tr("Aramaic");
    m_languages["arp"] = QObject::tr("Arapaho");
    m_languages["arn"] = QObject::tr("Araucanian");
    m_languages["arw"] = QObject::tr("Arawak");
    m_languages["arm"] = QObject::tr("Armenian");
    m_languages["art"] = QObject::tr("Artificial (Other)");
    m_languages["asm"] = QObject::tr("Assamese");
    m_languages["ath"] = QObject::tr("Athapascan languages");
    m_languages["map"] = QObject::tr("Austronesian (Other)");
    m_languages["ava"] = QObject::tr("Avaric");
    m_languages["ave"] = QObject::tr("Avestan");
    m_languages["awa"] = QObject::tr("Awadhi");
    m_languages["aym"] = QObject::tr("Aymara");
    m_languages["aze"] = QObject::tr("Azerbaijani");
    m_languages["nah"] = QObject::tr("Aztec");
    m_languages["ban"] = QObject::tr("Balinese");
    m_languages["bat"] = QObject::tr("Baltic (Other)");
    m_languages["bal"] = QObject::tr("Baluchi");
    m_languages["bam"] = QObject::tr("Bambara");
    m_languages["bai"] = QObject::tr("Bamileke languages");
    m_languages["bad"] = QObject::tr("Banda");
    m_languages["bnt"] = QObject::tr("Bantu (Other)");
    m_languages["bas"] = QObject::tr("Basa");
    m_languages["bak"] = QObject::tr("Bashkir");
    m_languages["baq"] = QObject::tr("Basque");
    m_languages["bej"] = QObject::tr("Beja");
    m_languages["bem"] = QObject::tr("Bemba");
    m_languages["ben"] = QObject::tr("Bengali");
    m_languages["ber"] = QObject::tr("Berber (Other)");
    m_languages["bho"] = QObject::tr("Bhojpuri");
    m_languages["bih"] = QObject::tr("Bihari");
    m_languages["bik"] = QObject::tr("Bikol");
    m_languages["bin"] = QObject::tr("Bini");
    m_languages["bis"] = QObject::tr("Bislama");
    m_languages["bra"] = QObject::tr("Braj");
    m_languages["bre"] = QObject::tr("Breton");
    m_languages["bug"] = QObject::tr("Buginese");
    m_languages["bul"] = QObject::tr("Bulgarian");
    m_languages["bua"] = QObject::tr("Buriat");
    m_languages["bur"] = QObject::tr("Burmese");
    m_languages["bel"] = QObject::tr("Byelorussian");
    m_languages["cad"] = QObject::tr("Caddo");
    m_languages["car"] = QObject::tr("Carib");
    m_languages["cat"] = QObject::tr("Catalan");
    m_languages["cau"] = QObject::tr("Caucasian (Other)");
    m_languages["ceb"] = QObject::tr("Cebuano");
    m_languages["cel"] = QObject::tr("Celtic (Other)");
    m_languages["cai"] = QObject::tr("Central American Indian (Other)");
    m_languages["chg"] = QObject::tr("Chagatai");
    m_languages["cha"] = QObject::tr("Chamorro");
    m_languages["che"] = QObject::tr("Chechen");
    m_languages["chr"] = QObject::tr("Cherokee");
    m_languages["chy"] = QObject::tr("Cheyenne");
    m_languages["chb"] = QObject::tr("Chibcha");
    m_languages["chi"] = QObject::tr("Chinese");
    m_languages["chn"] = QObject::tr("Chinook jargon");
    m_languages["cho"] = QObject::tr("Choctaw");
    m_languages["chu"] = QObject::tr("Church Slavic");
    m_languages["chv"] = QObject::tr("Chuvash");
    m_languages["cop"] = QObject::tr("Coptic");
    m_languages["cor"] = QObject::tr("Cornish");
    m_languages["cos"] = QObject::tr("Corsican");
    m_languages["cre"] = QObject::tr("Cree");
    m_languages["mus"] = QObject::tr("Creek");
    m_languages["crp"] = QObject::tr("Creoles and Pidgins (Other)");
    m_languages["cpe"] = QObject::tr("Creoles and Pidgins, English-based (Other)");
    m_languages["cpf"] = QObject::tr("Creoles and Pidgins, French-based (Other)");
    m_languages["cpp"] = QObject::tr("Creoles and Pidgins, Portuguese-based (Other)");
    m_languages["cus"] = QObject::tr("Cushitic (Other)");
    m_languages["ces"] = QObject::tr("Czech");
    m_languages["dak"] = QObject::tr("Dakota");
    m_languages["dan"] = QObject::tr("Danish");
    m_languages["del"] = QObject::tr("Delaware");
    m_languages["din"] = QObject::tr("Dinka");
    m_languages["div"] = QObject::tr("Divehi");
    m_languages["doi"] = QObject::tr("Dogri");
    m_languages["dra"] = QObject::tr("Dravidian (Other)");
    m_languages["dua"] = QObject::tr("Duala");
    m_languages["dut"] = QObject::tr("Dutch");
    m_languages["dum"] = QObject::tr("Dutch, Middle (ca. 1050-1350)");
    m_languages["dyu"] = QObject::tr("Dyula");
    m_languages["dzo"] = QObject::tr("Dzongkha");
    m_languages["efi"] = QObject::tr("Efik");
    m_languages["egy"] = QObject::tr("Egyptian (Ancient)");
    m_languages["eka"] = QObject::tr("Ekajuk");
    m_languages["elx"] = QObject::tr("Elamite");
    m_languages["eng"] = QObject::tr("English");
    m_languages["enm"] = QObject::tr("English, Middle (ca. 1100-1500)");
    m_languages["ang"] = QObject::tr("English, Old (ca. 450-1100)");
    m_languages["esk"] = QObject::tr("Eskimo (Other)");
    m_languages["epo"] = QObject::tr("Esperanto");
    m_languages["est"] = QObject::tr("Estonian");
    m_languages["ewe"] = QObject::tr("Ewe");
    m_languages["ewo"] = QObject::tr("Ewondo");
    m_languages["fan"] = QObject::tr("Fang");
    m_languages["fat"] = QObject::tr("Fanti");
    m_languages["fao"] = QObject::tr("Faroese");
    m_languages["fij"] = QObject::tr("Fijian");
    m_languages["fin"] = QObject::tr("Finnish");
    m_languages["fiu"] = QObject::tr("Finno-Ugrian (Other)");
    m_languages["fon"] = QObject::tr("Fon");
    m_languages["fre"] = QObject::tr("French");
    m_languages["frm"] = QObject::tr("French, Middle (ca. 1400-1600)");
    m_languages["fro"] = QObject::tr("French, Old (842- ca. 1400)");
    m_languages["fry"] = QObject::tr("Frisian");
    m_languages["ful"] = QObject::tr("Fulah");
    m_languages["gaa"] = QObject::tr("Ga");
    m_languages["gae"] = QObject::tr("Gaelic (Scots)");
    m_languages["glg"] = QObject::tr("Gallegan");
    m_languages["lug"] = QObject::tr("Ganda");
    m_languages["gay"] = QObject::tr("Gayo");
    m_languages["gez"] = QObject::tr("Geez");
    m_languages["geo"] = QObject::tr("Georgian");
    m_languages["deu"] = QObject::tr("German");
    m_languages["gmh"] = QObject::tr("German, Middle High (ca. 1050-1500)");
    m_languages["goh"] = QObject::tr("German, Old High (ca. 750-1050)");
    m_languages["gem"] = QObject::tr("Germanic (Other)");
    m_languages["gil"] = QObject::tr("Gilbertese");
    m_languages["gon"] = QObject::tr("Gondi");
    m_languages["got"] = QObject::tr("Gothic");
    m_languages["grb"] = QObject::tr("Grebo");
    m_languages["grc"] = QObject::tr("Greek, Ancient (to 1453)");
    m_languages["ell"] = QObject::tr("Greek, Modern (1453-)");
    m_languages["kal"] = QObject::tr("Greenlandic");
    m_languages["grn"] = QObject::tr("Guarani");
    m_languages["guj"] = QObject::tr("Gujarati");
    m_languages["hai"] = QObject::tr("Haida");
    m_languages["hau"] = QObject::tr("Hausa");
    m_languages["haw"] = QObject::tr("Hawaiian");
    m_languages["heb"] = QObject::tr("Hebrew");
    m_languages["her"] = QObject::tr("Herero");
    m_languages["hil"] = QObject::tr("Hiligaynon");
    m_languages["him"] = QObject::tr("Himachali");
    m_languages["hin"] = QObject::tr("Hindi");
    m_languages["hmo"] = QObject::tr("Hiri Motu");
    m_languages["hun"] = QObject::tr("Hungarian");
    m_languages["hup"] = QObject::tr("Hupa");
    m_languages["iba"] = QObject::tr("Iban");
    m_languages["ice"] = QObject::tr("Icelandic");
    m_languages["ibo"] = QObject::tr("Igbo");
    m_languages["ijo"] = QObject::tr("Ijo");
    m_languages["ilo"] = QObject::tr("Iloko");
    m_languages["inc"] = QObject::tr("Indic (Other)");
    m_languages["ine"] = QObject::tr("Indo-European (Other)");
    m_languages["ind"] = QObject::tr("Indonesian");
    m_languages["ina"] = QObject::tr("Interlingua (International Auxiliary language Association)");
    m_languages["ine"] = QObject::tr("- Interlingue");
    m_languages["iku"] = QObject::tr("Inuktitut");
    m_languages["ipk"] = QObject::tr("Inupiak");
    m_languages["ira"] = QObject::tr("Iranian (Other)");
    m_languages["gai"] = QObject::tr("Irish");
    m_languages["sga"] = QObject::tr("Irish, Old (to 900)");
    m_languages["mga"] = QObject::tr("Irish, Middle (900 - 1200)");
    m_languages["iro"] = QObject::tr("Iroquoian languages");
    m_languages["ita"] = QObject::tr("Italian");
    m_languages["jpn"] = QObject::tr("Japanese");
    m_languages["jav"] = QObject::tr("Javanese");
    m_languages["jrb"] = QObject::tr("Judeo-Arabic");
    m_languages["jpr"] = QObject::tr("Judeo-Persian");
    m_languages["kab"] = QObject::tr("Kabyle");
    m_languages["kac"] = QObject::tr("Kachin");
    m_languages["kam"] = QObject::tr("Kamba");
    m_languages["kan"] = QObject::tr("Kannada");
    m_languages["kau"] = QObject::tr("Kanuri");
    m_languages["kaa"] = QObject::tr("Kara-Kalpak");
    m_languages["kar"] = QObject::tr("Karen");
    m_languages["kas"] = QObject::tr("Kashmiri");
    m_languages["kaw"] = QObject::tr("Kawi");
    m_languages["kaz"] = QObject::tr("Kazakh");
    m_languages["kha"] = QObject::tr("Khasi");
    m_languages["khm"] = QObject::tr("Khmer");
    m_languages["khi"] = QObject::tr("Khoisan (Other)");
    m_languages["kho"] = QObject::tr("Khotanese");
    m_languages["kik"] = QObject::tr("Kikuyu");
    m_languages["kin"] = QObject::tr("Kinyarwanda");
    m_languages["kir"] = QObject::tr("Kirghiz");
    m_languages["kom"] = QObject::tr("Komi");
    m_languages["kon"] = QObject::tr("Kongo");
    m_languages["kok"] = QObject::tr("Konkani");
    m_languages["kor"] = QObject::tr("Korean");
    m_languages["kpe"] = QObject::tr("Kpelle");
    m_languages["kro"] = QObject::tr("Kru");
    m_languages["kua"] = QObject::tr("Kuanyama");
    m_languages["kum"] = QObject::tr("Kumyk");
    m_languages["kur"] = QObject::tr("Kurdish");
    m_languages["kru"] = QObject::tr("Kurukh");
    m_languages["kus"] = QObject::tr("Kusaie");
    m_languages["kut"] = QObject::tr("Kutenai");
    m_languages["lad"] = QObject::tr("Ladino");
    m_languages["lah"] = QObject::tr("Lahnda");
    m_languages["lam"] = QObject::tr("Lamba");
    m_languages["oci"] = QObject::tr("Langue d'Oc (post 1500)");
    m_languages["lao"] = QObject::tr("Lao");
    m_languages["lat"] = QObject::tr("Latin");
    m_languages["lav"] = QObject::tr("Latvian");
    m_languages["ltz"] = QObject::tr("Letzeburgesch");
    m_languages["lez"] = QObject::tr("Lezghian");
    m_languages["lin"] = QObject::tr("Lingala");
    m_languages["lit"] = QObject::tr("Lithuanian");
    m_languages["loz"] = QObject::tr("Lozi");
    m_languages["lub"] = QObject::tr("Luba-Katanga");
    m_languages["lui"] = QObject::tr("Luiseno");
    m_languages["lun"] = QObject::tr("Lunda");
    m_languages["luo"] = QObject::tr("Luo (Kenya and Tanzania)");
    m_languages["mac"] = QObject::tr("Macedonian");
    m_languages["mad"] = QObject::tr("Madurese");
    m_languages["mag"] = QObject::tr("Magahi");
    m_languages["mai"] = QObject::tr("Maithili");
    m_languages["mak"] = QObject::tr("Makasar");
    m_languages["mlg"] = QObject::tr("Malagasy");
    m_languages["may"] = QObject::tr("Malay");
    m_languages["mal"] = QObject::tr("Malayalam");
    m_languages["mlt"] = QObject::tr("Maltese");
    m_languages["man"] = QObject::tr("Mandingo");
    m_languages["mni"] = QObject::tr("Manipuri");
    m_languages["mno"] = QObject::tr("Manobo languages");
    m_languages["max"] = QObject::tr("Manx");
    m_languages["mao"] = QObject::tr("Maori");
    m_languages["mar"] = QObject::tr("Marathi");
    m_languages["chm"] = QObject::tr("Mari");
    m_languages["mah"] = QObject::tr("Marshall");
    m_languages["mwr"] = QObject::tr("Marwari");
    m_languages["mas"] = QObject::tr("Masai");
    m_languages["myn"] = QObject::tr("Mayan languages");
    m_languages["men"] = QObject::tr("Mende");
    m_languages["mic"] = QObject::tr("Micmac");
    m_languages["min"] = QObject::tr("Minangkabau");
    m_languages["mis"] = QObject::tr("Miscellaneous (Other)");
    m_languages["moh"] = QObject::tr("Mohawk");
    m_languages["mol"] = QObject::tr("Moldavian");
    m_languages["mkh"] = QObject::tr("Mon-Kmer (Other)");
    m_languages["lol"] = QObject::tr("Mongo");
    m_languages["mon"] = QObject::tr("Mongolian");
    m_languages["mos"] = QObject::tr("Mossi");
    m_languages["mul"] = QObject::tr("Multiple languages");
    m_languages["mun"] = QObject::tr("Munda languages");
    m_languages["nau"] = QObject::tr("Nauru");
    m_languages["nav"] = QObject::tr("Navajo");
    m_languages["nde"] = QObject::tr("Ndebele, North");
    m_languages["nbl"] = QObject::tr("Ndebele, South");
    m_languages["ndo"] = QObject::tr("Ndongo");
    m_languages["nep"] = QObject::tr("Nepali");
    m_languages["new"] = QObject::tr("Newari");
    m_languages["nic"] = QObject::tr("Niger-Kordofanian (Other)");
    m_languages["ssa"] = QObject::tr("Nilo-Saharan (Other)");
    m_languages["niu"] = QObject::tr("Niuean");
    m_languages["non"] = QObject::tr("Norse, Old");
    m_languages["nai"] = QObject::tr("North American Indian (Other)");
    m_languages["nor"] = QObject::tr("Norwegian");
    m_languages["nno"] = QObject::tr("Norwegian (Nynorsk)");
    m_languages["nub"] = QObject::tr("Nubian languages");
    m_languages["nym"] = QObject::tr("Nyamwezi");
    m_languages["nya"] = QObject::tr("Nyanja");
    m_languages["nyn"] = QObject::tr("Nyankole");
    m_languages["nyo"] = QObject::tr("Nyoro");
    m_languages["nzi"] = QObject::tr("Nzima");
    m_languages["oji"] = QObject::tr("Ojibwa");
    m_languages["ori"] = QObject::tr("Oriya");
    m_languages["orm"] = QObject::tr("Oromo");
    m_languages["osa"] = QObject::tr("Osage");
    m_languages["oss"] = QObject::tr("Ossetic");
    m_languages["oto"] = QObject::tr("Otomian languages");
    m_languages["pal"] = QObject::tr("Pahlavi");
    m_languages["pau"] = QObject::tr("Palauan");
    m_languages["pli"] = QObject::tr("Pali");
    m_languages["pam"] = QObject::tr("Pampanga");
    m_languages["pag"] = QObject::tr("Pangasinan");
    m_languages["pan"] = QObject::tr("Panjabi");
    m_languages["pap"] = QObject::tr("Papiamento");
    m_languages["paa"] = QObject::tr("Papuan-Australian (Other)");
    m_languages["fas"] = QObject::tr("Persian");
    m_languages["peo"] = QObject::tr("Persian, Old (ca 600 - 400 B.C.)");
    m_languages["phn"] = QObject::tr("Phoenician");
    m_languages["pol"] = QObject::tr("Polish");
    m_languages["pon"] = QObject::tr("Ponape");
    m_languages["por"] = QObject::tr("Portuguese");
    m_languages["pra"] = QObject::tr("Prakrit languages");
    m_languages["pro"] = QObject::tr("Provencal, Old (to 1500)");
    m_languages["pus"] = QObject::tr("Pushto");
    m_languages["que"] = QObject::tr("Quechua");
    m_languages["roh"] = QObject::tr("Rhaeto-Romance");
    m_languages["raj"] = QObject::tr("Rajasthani");
    m_languages["rar"] = QObject::tr("Rarotongan");
    m_languages["roa"] = QObject::tr("Romance (Other)");
    m_languages["ron"] = QObject::tr("Romanian");
    m_languages["rom"] = QObject::tr("Romany");
    m_languages["run"] = QObject::tr("Rundi");
    m_languages["rus"] = QObject::tr("Russian");
    m_languages["sal"] = QObject::tr("Salishan languages");
    m_languages["sam"] = QObject::tr("Samaritan Aramaic");
    m_languages["smi"] = QObject::tr("Sami languages");
    m_languages["smo"] = QObject::tr("Samoan");
    m_languages["sad"] = QObject::tr("Sandawe");
    m_languages["sag"] = QObject::tr("Sango");
    m_languages["san"] = QObject::tr("Sanskrit");
    m_languages["srd"] = QObject::tr("Sardinian");
    m_languages["sco"] = QObject::tr("Scots");
    m_languages["sel"] = QObject::tr("Selkup");
    m_languages["sem"] = QObject::tr("Semitic (Other)");
    m_languages["scc"] = QObject::tr("Serbian");
    m_languages["scr"] = QObject::tr("Serbo-Croatian");
    m_languages["srr"] = QObject::tr("Serer");
    m_languages["shn"] = QObject::tr("Shan");
    m_languages["sna"] = QObject::tr("Shona");
    m_languages["sid"] = QObject::tr("Sidamo");
    m_languages["bla"] = QObject::tr("Siksika");
    m_languages["snd"] = QObject::tr("Sindhi");
    m_languages["sin"] = QObject::tr("Singhalese");
    m_languages["sit"] = QObject::tr("- Sino-Tibetan (Other)");
    m_languages["sio"] = QObject::tr("Siouan languages");
    m_languages["sla"] = QObject::tr("Slavic (Other)");
    m_languages["ssw"] = QObject::tr("Siswant");
    m_languages["slk"] = QObject::tr("Slovak");
    m_languages["slv"] = QObject::tr("Slovenian");
    m_languages["sog"] = QObject::tr("Sogdian");
    m_languages["som"] = QObject::tr("Somali");
    m_languages["son"] = QObject::tr("Songhai");
    m_languages["wen"] = QObject::tr("Sorbian languages");
    m_languages["nso"] = QObject::tr("Sotho, Northern");
    m_languages["sot"] = QObject::tr("Sotho, Southern");
    m_languages["sai"] = QObject::tr("South American Indian (Other)");
    m_languages["esl"] = QObject::tr("Spanish");
    m_languages["suk"] = QObject::tr("Sukuma");
    m_languages["sux"] = QObject::tr("Sumerian");
    m_languages["sun"] = QObject::tr("Sudanese");
    m_languages["sus"] = QObject::tr("Susu");
    m_languages["swa"] = QObject::tr("Swahili");
    m_languages["ssw"] = QObject::tr("Swazi");
    m_languages["sve"] = QObject::tr("Swedish");
    m_languages["syr"] = QObject::tr("Syriac");
    m_languages["tgl"] = QObject::tr("Tagalog");
    m_languages["tah"] = QObject::tr("Tahitian");
    m_languages["tgk"] = QObject::tr("Tajik");
    m_languages["tmh"] = QObject::tr("Tamashek");
    m_languages["tam"] = QObject::tr("Tamil");
    m_languages["tat"] = QObject::tr("Tatar");
    m_languages["tel"] = QObject::tr("Telugu");
    m_languages["ter"] = QObject::tr("Tereno");
    m_languages["tha"] = QObject::tr("Thai");
    m_languages["bod"] = QObject::tr("Tibetan");
    m_languages["tig"] = QObject::tr("Tigre");
    m_languages["tir"] = QObject::tr("Tigrinya");
    m_languages["tem"] = QObject::tr("Timne");
    m_languages["tiv"] = QObject::tr("Tivi");
    m_languages["tli"] = QObject::tr("Tlingit");
    m_languages["tog"] = QObject::tr("Tonga (Nyasa)");
    m_languages["ton"] = QObject::tr("Tonga (Tonga Islands)");
    m_languages["tru"] = QObject::tr("Truk");
    m_languages["tsi"] = QObject::tr("Tsimshian");
    m_languages["tso"] = QObject::tr("Tsonga");
    m_languages["tsn"] = QObject::tr("Tswana");
    m_languages["tum"] = QObject::tr("Tumbuka");
    m_languages["tur"] = QObject::tr("Turkish");
    m_languages["ota"] = QObject::tr("Turkish, Ottoman (1500 - 1928)");
    m_languages["tuk"] = QObject::tr("Turkmen");
    m_languages["tyv"] = QObject::tr("Tuvinian");
    m_languages["twi"] = QObject::tr("Twi");
    m_languages["uga"] = QObject::tr("Ugaritic");
    m_languages["uig"] = QObject::tr("Uighur");
    m_languages["ukr"] = QObject::tr("Ukrainian");
    m_languages["umb"] = QObject::tr("Umbundu");
    m_languages["und"] = QObject::tr("Undetermined");
    m_languages["urd"] = QObject::tr("Urdu");
    m_languages["uzb"] = QObject::tr("Uzbek");
    m_languages["vai"] = QObject::tr("Vai");
    m_languages["ven"] = QObject::tr("Venda");
    m_languages["vie"] = QObject::tr("Vietnamese");
    m_languages["vol"] = QObject::tr("Volapuk");
    m_languages["vot"] = QObject::tr("Votic");
    m_languages["wak"] = QObject::tr("Wakashan languages");
    m_languages["wal"] = QObject::tr("Walamo");
    m_languages["war"] = QObject::tr("Waray");
    m_languages["was"] = QObject::tr("Washo");
    m_languages["cym"] = QObject::tr("Welsh");
    m_languages["wol"] = QObject::tr("Wolof");
    m_languages["xho"] = QObject::tr("Xhosa");
    m_languages["sah"] = QObject::tr("Yakut");
    m_languages["yao"] = QObject::tr("Yao");
    m_languages["yap"] = QObject::tr("Yap");
    m_languages["yid"] = QObject::tr("Yiddish");
    m_languages["yor"] = QObject::tr("Yoruba");
    m_languages["zap"] = QObject::tr("Zapotec");
    m_languages["zen"] = QObject::tr("Zenaga");
    m_languages["zha"] = QObject::tr("Zhuang");
    m_languages["zul"] = QObject::tr("Zulu");
    m_languages["zun"] = QObject::tr("Zuni");
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

