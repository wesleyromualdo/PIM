ï»¿/*==================================================
 *  Japanese Era Date Labeller
 *==================================================
 */

Timeline.JapaneseEraDateLabeller = function(locale, timeZone, useRomanizedName) {
    var o = new Timeline.GregorianDateLabeller(locale, timeZone);
    
    o._useRomanizedName = (useRomanizedName);
    o._oldLabelInterval = o.labelInterval;
    o.labelInterval = Timeline.JapaneseEraDateLabeller._labelInterval;
    
    return o;
};

Timeline.JapaneseEraDateLabeller._labelInterval = function(date, intervalUnit) {
    var text;
    var emphasized = false;
    
    var date2 = Timeline.DateTime.removeTimeZoneOffset(date, this._timeZone);
    
    switch(intervalUnit) {
    case Timeline.DateTime.YEAR:
    case Timeline.DateTime.DECADE:
    case Timeline.DateTime.CENTURY:
    case Timeline.DateTime.MILLENNIUM:
        var y = date2.getUTCFullYear();
        if (y >= Timeline.JapaneseEraDateLabeller._eras.elementAt(0).startingYear) {
            var eraIndex = Timeline.JapaneseEraDateLabeller._eras.find(function(era) {
                    return era.startingYear - y;
                }
            );
            if (eraIndex < Timeline.JapaneseEraDateLabeller._eras.length()) {
                var era = Timeline.JapaneseEraDateLabeller._eras.elementAt(eraIndex);
                if (y < era.startingYear) {
                    era = Timeline.JapaneseEraDateLabeller._eras.elementAt(eraIndex - 1);
                }
            } else {
                var era = Timeline.JapaneseEraDateLabeller._eras.elementAt(eraIndex - 1);
            }
            
            text = (this._useRomanizedName ? era.romanizedName : era.japaneseName) + " " + (y - era.startingYear + 1);
            emphasized = intervalUnit == Timeline.DateTime.YEAR && y == era.startingYear;
            break;
        } // else, fall through
    default:
        return this._oldLabelInterval(date, intervalUnit);
    }
    
    return { text: text, emphasized: emphasized };
};

/*==================================================
 *  Japanese Era Ether Painter
 *==================================================
 */
 
Timeline.JapaneseEraEtherPainter = function(params, band, timeline) {
    this._params = params;
    this._theme = params.theme;
};

Timeline.JapaneseEraEtherPainter.prototype.initialize = function(band, timeline) {
    this._band = band;
    this._timeline = timeline;
    
    this._backgroundLayer = band.createLayerDiv(0);
    this._backgroundLayer.setAttribute("name", "ether-background"); // for debugging
    this._backgroundLayer.style.background = this._theme.ether.backgroundColors[band.getIndex()];
    
    this._markerLayer = null;
    this._lineLayer = null;
    
    var align = ("align" in this._params) ? this._params.align : 
        this._theme.ether.interval.marker[timeline.isHorizontal() ? "hAlign" : "vAlign"];
    var showLine = ("showLine" in this._params) ? this._params.showLine : 
        this._theme.ether.interval.line.show;
        
    this._intervalMarkerLayout = new Timeline.EtherIntervalMarkerLayout(
        this._timeline, this._band, this._theme, align, showLine);
        
    this._highlight = new Timeline.EtherHighlight(
        this._timeline, this._band, this._theme, this._backgroundLayer);
}

Timeline.JapaneseEraEtherPainter.prototype.setHighlight = function(startDate, endDate) {
    this._highlight.position(startDate, endDate);
}

Timeline.JapaneseEraEtherPainter.prototype.paint = function() {
    if (this._markerLayer) {
        this._band.removeLayerDiv(this._markerLayer);
    }
    this._markerLayer = this._band.createLayerDiv(100);
    this._markerLayer.setAttribute("name", "ether-markers"); // for debugging
    this._markerLayer.style.display = "none";
    
    if (this._lineLayer) {
        this._band.removeLayerDiv(this._lineLayer);
    }
    this._lineLayer = this._band.createLayerDiv(1);
    this._lineLayer.setAttribute("name", "ether-lines"); // for debugging
    this._lineLayer.style.display = "none";
    
    var minYear = this._band.getMinDate().getUTCFullYear();
    var maxYear = this._band.getMaxDate().getUTCFullYear();
    var eraIndex = Timeline.JapaneseEraDateLabeller._eras.find(function(era) {
            return era.startingYear - minYear;
        }
    );
    
    var l = Timeline.JapaneseEraDateLabeller._eras.length();
    for (var i = eraIndex; i < l; i++) {
        var era = Timeline.JapaneseEraDateLabeller._eras.elementAt(i);
        if (era.startingYear > maxYear) {
            break;
        }
        
        var d = new Date(0);
        d.setUTCFullYear(era.startingYear);
        
        var labeller = {
            labelInterval: function(date, intervalUnit) {
                return {
                    text: era.japaneseName,
                    emphasized: true
                };
            }
        };
        
        this._intervalMarkerLayout.createIntervalMarker(
            d, labeller, Timeline.DateTime.YEAR, this._markerLayer, this._lineLayer);
    }
    this._markerLayer.style.display = "block";
    this._lineLayer.style.display = "block";
};

Timeline.JapaneseEraEtherPainter.prototype.softPaint = function() {
};


Timeline.JapaneseEraDateLabeller._eras = new Timeline.SortedArray(
    function(e1, e2) {
        return e1.startingYear - e2.startingYear;
    },
    [
        { startingYear: 645, japaneseName: 'å¤§å', romanizedName: "Taika" },
        { startingYear: 650, japaneseName: 'ç½é', romanizedName: "Hakuchi" },
        { startingYear: 686, japaneseName: 'æ±é³¥', romanizedName: "ShuchÅ" },
        { startingYear: 701, japaneseName: 'å¤§å®', romanizedName: "TaihÅ" },
        { startingYear: 704, japaneseName: 'æ¶é²', romanizedName: "Keiun" },
        { startingYear: 708, japaneseName: 'åé', romanizedName: "WadÅ" },
        { startingYear: 715, japaneseName: 'éäº', romanizedName: "Reiki" },
        { startingYear: 717, japaneseName: 'é¤è', romanizedName: "YÅrÅ" },
        { startingYear: 724, japaneseName: 'ç¥äº', romanizedName: "Jinki" },
        { startingYear: 729, japaneseName: 'å¤©å¹³', romanizedName: "TenpyÅ" },
        { startingYear: 749, japaneseName: 'å¤©å¹³æå®', romanizedName: "TenpyÅ-kanpÅ" },
        { startingYear: 749, japaneseName: 'å¤©å¹³åå®', romanizedName: "TenpyÅ-shÅhÅ" },
        { startingYear: 757, japaneseName: 'å¤©å¹³å®å­', romanizedName: "TenpyÅ-hÅji" },
        { startingYear: 765, japaneseName: 'å¤©å¹³ç¥è­·', romanizedName: "TenpyÅ-jingo" },
        { startingYear: 767, japaneseName: 'ç¥è­·æ¯é²', romanizedName: "Jingo-keiun" },
        { startingYear: 770, japaneseName: 'å®äº', romanizedName: "HÅki" },
        { startingYear: 781, japaneseName: 'å¤©å¿', romanizedName: "Ten'Å" },
        { startingYear: 782, japaneseName: 'å»¶æ¦', romanizedName: "Enryaku" },
        { startingYear: 806, japaneseName: 'å¤§å', romanizedName: "DaidÅ" },
        { startingYear: 810, japaneseName: 'å¼ä»', romanizedName: "KÅnin" },
        { startingYear: 824, japaneseName: 'å¤©é·', romanizedName: "TenchÅ" },
        { startingYear: 834, japaneseName: 'æ¿å', romanizedName: "JÅwa" },
        { startingYear: 848, japaneseName: 'åç¥¥', romanizedName: "KajÅ" },
        { startingYear: 851, japaneseName: 'ä»å¯¿', romanizedName: "Ninju" },
        { startingYear: 854, japaneseName: 'æè¡¡', romanizedName: "SaikÅ" },
        { startingYear: 857, japaneseName: 'å¤©å®', romanizedName: "Tennan" },
        { startingYear: 859, japaneseName: 'è²è¦³', romanizedName: "JÅgan" },
        { startingYear: 877, japaneseName: 'åæ¶', romanizedName: "GangyÅ" },
        { startingYear: 885, japaneseName: 'ä»å', romanizedName: "Ninna" },
        { startingYear: 889, japaneseName: 'å¯å¹³', romanizedName: "KanpyÅ" },
        { startingYear: 898, japaneseName: 'ææ³°', romanizedName: "ShÅtai" },
        { startingYear: 901, japaneseName: 'å»¶å', romanizedName: "Engi" },
        { startingYear: 923, japaneseName: 'å»¶é·', romanizedName: "EnchÅ" },
        { startingYear: 931, japaneseName: 'æ¿å¹³', romanizedName: "JÅhei" },
        { startingYear: 938, japaneseName: 'å¤©æ¶', romanizedName: "TengyÅ" },
        { startingYear: 947, japaneseName: 'å¤©æ¦', romanizedName: "Tenryaku" },
        { startingYear: 957, japaneseName: 'å¤©å¾³', romanizedName: "Tentoku" },
        { startingYear: 961, japaneseName: 'å¿å', romanizedName: "Åwa" },
        { startingYear: 964, japaneseName: 'åº·ä¿', romanizedName: "KÅhÅ" },
        { startingYear: 968, japaneseName: 'å®å', romanizedName: "Anna" },
        { startingYear: 970, japaneseName: 'å¤©ç¦', romanizedName: "Tenroku" },
        { startingYear: 973, japaneseName: 'å¤©å»¶', romanizedName: "Ten'en" },
        { startingYear: 976, japaneseName: 'è²å', romanizedName: "JÅgen" },
        { startingYear: 978, japaneseName: 'å¤©å', romanizedName: "Tengen" },
        { startingYear: 983, japaneseName: 'æ°¸è¦³', romanizedName: "Eikan" },
        { startingYear: 985, japaneseName: 'å¯å', romanizedName: "Kanna" },
        { startingYear: 987, japaneseName: 'æ°¸å»¶', romanizedName: "Eien" },
        { startingYear: 988, japaneseName: 'æ°¸ç¥', romanizedName: "Eiso" },
        { startingYear: 990, japaneseName: 'æ­£æ¦', romanizedName: "ShÅryaku" },
        { startingYear: 995, japaneseName: 'é·å¾³', romanizedName: "ChÅtoku" },
        { startingYear: 999, japaneseName: 'é·ä¿', romanizedName: "ChÅhÅ" },
        { startingYear: 1004, japaneseName: 'å¯å¼', romanizedName: "KankÅ" },
        { startingYear: 1012, japaneseName: 'é·å', romanizedName: "ChÅwa" },
        { startingYear: 1017, japaneseName: 'å¯ä»', romanizedName: "Kannin" },
        { startingYear: 1021, japaneseName: 'æ²»å®', romanizedName: "Jian" },
        { startingYear: 1024, japaneseName: 'ä¸å¯¿', romanizedName: "Manju" },
        { startingYear: 1028, japaneseName: 'é·å', romanizedName: "ChÅgen" },
        { startingYear: 1037, japaneseName: 'é·æ¦', romanizedName: "ChÅryaku" },
        { startingYear: 1040, japaneseName: 'é·ä¹', romanizedName: "ChÅkyÅ«" },
        { startingYear: 1044, japaneseName: 'å¯å¾³', romanizedName: "Kantoku" },
        { startingYear: 1046, japaneseName: 'æ°¸æ¿', romanizedName: "EishÅ" },
        { startingYear: 1053, japaneseName: 'å¤©å', romanizedName: "Tengi" },
        { startingYear: 1058, japaneseName: 'åº·å¹³', romanizedName: "KÅhei" },
        { startingYear: 1065, japaneseName: 'æ²»æ¦', romanizedName: "Jiryaku" },
        { startingYear: 1069, japaneseName: 'å»¶ä¹', romanizedName: "EnkyÅ«" },
        { startingYear: 1074, japaneseName: 'æ¿ä¿', romanizedName: "JÅhÅ" },
        { startingYear: 1077, japaneseName: 'æ¿æ¦', romanizedName: "JÅryaku" },
        { startingYear: 1081, japaneseName: 'æ°¸ä¿', romanizedName: "EihÅ" },
        { startingYear: 1084, japaneseName: 'å¿å¾³', romanizedName: "Åtoku" },
        { startingYear: 1087, japaneseName: 'å¯æ²»', romanizedName: "Kanji" },
        { startingYear: 1094, japaneseName: 'åä¿', romanizedName: "KahÅ" },
        { startingYear: 1096, japaneseName: 'æ°¸é·', romanizedName: "EichÅ" },
        { startingYear: 1097, japaneseName: 'æ¿å¾³', romanizedName: "JÅtoku" },
        { startingYear: 1099, japaneseName: 'åº·å', romanizedName: "KÅwa" },
        { startingYear: 1104, japaneseName: 'é·æ²»', romanizedName: "ChÅji" },
        { startingYear: 1106, japaneseName: 'åæ¿', romanizedName: "KajÅ" },
        { startingYear: 1108, japaneseName: 'å¤©ä»', romanizedName: "Tennin" },
        { startingYear: 1110, japaneseName: 'å¤©æ°¸', romanizedName: "Ten'ei" },
        { startingYear: 1113, japaneseName: 'æ°¸ä¹', romanizedName: "EikyÅ«" },
        { startingYear: 1118, japaneseName: 'åæ°¸', romanizedName: "Gen'ei" },
        { startingYear: 1120, japaneseName: 'ä¿å®', romanizedName: "HÅan" },
        { startingYear: 1124, japaneseName: 'å¤©æ²»', romanizedName: "Tenji" },
        { startingYear: 1126, japaneseName: 'å¤§æ²»', romanizedName: "Daiji" },
        { startingYear: 1131, japaneseName: 'å¤©æ¿', romanizedName: "TenshÅ" },
        { startingYear: 1132, japaneseName: 'é·æ¿', romanizedName: "ChÅshÅ" },
        { startingYear: 1135, japaneseName: 'ä¿å»¶', romanizedName: "HÅen" },
        { startingYear: 1141, japaneseName: 'æ°¸æ²»', romanizedName: "Eiji" },
        { startingYear: 1142, japaneseName: 'åº·æ²»', romanizedName: "KÅji" },
        { startingYear: 1144, japaneseName: 'å¤©é¤', romanizedName: "Ten'yÅ" },
        { startingYear: 1145, japaneseName: 'ä¹å®', romanizedName: "KyÅ«an" },
        { startingYear: 1151, japaneseName: 'ä»å¹³', romanizedName: "Ninpei" },
        { startingYear: 1154, japaneseName: 'ä¹å¯¿', romanizedName: "KyÅ«ju" },
        { startingYear: 1156, japaneseName: 'ä¿å', romanizedName: "HÅgen" },
        { startingYear: 1159, japaneseName: 'å¹³æ²»', romanizedName: "Heiji" },
        { startingYear: 1160, japaneseName: 'æ°¸æ¦', romanizedName: "Eiryaku" },
        { startingYear: 1161, japaneseName: 'å¿ä¿', romanizedName: "ÅhÅ" },
        { startingYear: 1163, japaneseName: 'é·å¯', romanizedName: "ChÅkan" },
        { startingYear: 1165, japaneseName: 'æ°¸ä¸', romanizedName: "Eiman" },
        { startingYear: 1166, japaneseName: 'ä»å®', romanizedName: "Ninnan" },
        { startingYear: 1169, japaneseName: 'åå¿', romanizedName: "KaÅ" },
        { startingYear: 1171, japaneseName: 'æ¿å®', romanizedName: "JÅan" },
        { startingYear: 1175, japaneseName: 'å®å', romanizedName: "Angen" },
        { startingYear: 1177, japaneseName: 'æ²»æ¿', romanizedName: "JishÅ" },
        { startingYear: 1181, japaneseName: 'é¤å', romanizedName: "YÅwa" },
        { startingYear: 1182, japaneseName: 'å¯¿æ°¸', romanizedName: "Juei" },
        { startingYear: 1184, japaneseName: 'åæ¦', romanizedName: "Genryaku" },
        { startingYear: 1185, japaneseName: 'ææ²»', romanizedName: "Bunji" },
        { startingYear: 1190, japaneseName: 'å»ºä¹', romanizedName: "KenkyÅ«" },
        { startingYear: 1199, japaneseName: 'æ­£æ²»', romanizedName: "ShÅji" },
        { startingYear: 1201, japaneseName: 'å»ºä»', romanizedName: "Kennin" },
        { startingYear: 1204, japaneseName: 'åä¹', romanizedName: "GenkyÅ«" },
        { startingYear: 1206, japaneseName: 'å»ºæ°¸', romanizedName: "Ken'ei" },
        { startingYear: 1207, japaneseName: 'æ¿å', romanizedName: "JÅgen" },
        { startingYear: 1211, japaneseName: 'å»ºæ¦', romanizedName: "Kenryaku" },
        { startingYear: 1213, japaneseName: 'å»ºä¿', romanizedName: "KenpÅ" },
        { startingYear: 1219, japaneseName: 'æ¿ä¹', romanizedName: "JÅkyÅ«" },
        { startingYear: 1222, japaneseName: 'è²å¿', romanizedName: "JÅÅ" },
        { startingYear: 1224, japaneseName: 'åä»', romanizedName: "Gennin" },
        { startingYear: 1225, japaneseName: 'åç¦', romanizedName: "Karoku" },
        { startingYear: 1227, japaneseName: 'å®è²', romanizedName: "Antei" },
        { startingYear: 1229, japaneseName: 'å¯å', romanizedName: "Kanki" },
        { startingYear: 1232, japaneseName: 'è²æ°¸', romanizedName: "JÅei" },
        { startingYear: 1233, japaneseName: 'å¤©ç¦', romanizedName: "Tenpuku" },
        { startingYear: 1234, japaneseName: 'ææ¦', romanizedName: "Bunryaku" },
        { startingYear: 1235, japaneseName: 'åç¦', romanizedName: "Katei" },
        { startingYear: 1238, japaneseName: 'æ¦ä»', romanizedName: "Ryakunin" },
        { startingYear: 1239, japaneseName: 'å»¶å¿', romanizedName: "En'Å" },
        { startingYear: 1240, japaneseName: 'ä»æ²»', romanizedName: "Ninji" },
        { startingYear: 1243, japaneseName: 'å¯å', romanizedName: "Kangen" },
        { startingYear: 1247, japaneseName: 'å®æ²»', romanizedName: "HÅji" },
        { startingYear: 1249, japaneseName: 'å»ºé·', romanizedName: "KenchÅ" },
        { startingYear: 1256, japaneseName: 'åº·å', romanizedName: "KÅgen" },
        { startingYear: 1257, japaneseName: 'æ­£å', romanizedName: "ShÅka" },
        { startingYear: 1259, japaneseName: 'æ­£å', romanizedName: "ShÅgen" },
        { startingYear: 1260, japaneseName: 'æå¿', romanizedName: "Bun'Å" },
        { startingYear: 1261, japaneseName: 'å¼é·', romanizedName: "KÅcho" },
        { startingYear: 1264, japaneseName: 'ææ°¸', romanizedName: "Bun'ei" },
        { startingYear: 1275, japaneseName: 'å»ºæ²»', romanizedName: "Kenji" },
        { startingYear: 1278, japaneseName: 'å¼å®', romanizedName: "KÅan" },
        { startingYear: 1288, japaneseName: 'æ­£å¿', romanizedName: "ShÅÅ" },
        { startingYear: 1293, japaneseName: 'æ°¸ä»', romanizedName: "Einin" },
        { startingYear: 1299, japaneseName: 'æ­£å®', romanizedName: "ShÅan" },
        { startingYear: 1302, japaneseName: 'ä¹¾å', romanizedName: "Kengen" },
        { startingYear: 1303, japaneseName: 'åå', romanizedName: "Kagen" },
        { startingYear: 1306, japaneseName: 'å¾³æ²»', romanizedName: "Tokuji" },
        { startingYear: 1308, japaneseName: 'å»¶æ¶', romanizedName: "Enkei" },
        { startingYear: 1311, japaneseName: 'å¿é·', romanizedName: "ÅchÅ" },
        { startingYear: 1312, japaneseName: 'æ­£å', romanizedName: "ShÅwa" },
        { startingYear: 1317, japaneseName: 'æä¿', romanizedName: "BunpÅ" },
        { startingYear: 1319, japaneseName: 'åå¿', romanizedName: "Gen'Å" },
        { startingYear: 1321, japaneseName: 'åäº¨', romanizedName: "GenkyÅ" },
        { startingYear: 1324, japaneseName: 'æ­£ä¸­', romanizedName: "ShÅchÅ«" },
        { startingYear: 1326, japaneseName: 'åæ¦', romanizedName: "Karyaku" },
        { startingYear: 1329, japaneseName: 'åå¾³', romanizedName: "Gentoku" },
        { startingYear: 1331, japaneseName: 'åå¼', romanizedName: "GenkÅ" },
        { startingYear: 1334, japaneseName: 'å»ºæ­¦', romanizedName: "Kenmu" },
        { startingYear: 1336, japaneseName: 'å»¶å', romanizedName: "Engen" },
        { startingYear: 1340, japaneseName: 'èå½', romanizedName: "KÅkoku" },
        { startingYear: 1346, japaneseName: 'æ­£å¹³', romanizedName: "ShÅhei" },
        { startingYear: 1370, japaneseName: 'å»ºå¾³', romanizedName: "Kentoku" },
        { startingYear: 1372, japaneseName: 'æä¸­', romanizedName: "BunchÅ«" },
        { startingYear: 1375, japaneseName: 'å¤©æ', romanizedName: "Tenju" },
        { startingYear: 1381, japaneseName: 'å¼å', romanizedName: "KÅwa" },
        { startingYear: 1384, japaneseName: 'åä¸­', romanizedName: "GenchÅ«" },
        { startingYear: 1332, japaneseName: 'æ­£æ¶', romanizedName: "ShÅkei" },
        { startingYear: 1338, japaneseName: 'æ¦å¿', romanizedName: "RyakuÅ" },
        { startingYear: 1342, japaneseName: 'åº·æ°¸', romanizedName: "KÅei" },
        { startingYear: 1345, japaneseName: 'è²å', romanizedName: "JÅwa" },
        { startingYear: 1350, japaneseName: 'è¦³å¿', romanizedName: "Kan'Å" },
        { startingYear: 1352, japaneseName: 'æå', romanizedName: "Bunna" },
        { startingYear: 1356, japaneseName: 'å»¶æ', romanizedName: "Enbun" },
        { startingYear: 1361, japaneseName: 'åº·å®', romanizedName: "KÅan" },
        { startingYear: 1362, japaneseName: 'è²æ²»', romanizedName: "JÅji" },
        { startingYear: 1368, japaneseName: 'å¿å®', romanizedName: "Åan" },
        { startingYear: 1375, japaneseName: 'æ°¸å', romanizedName: "Eiwa" },
        { startingYear: 1379, japaneseName: 'åº·æ¦', romanizedName: "KÅryaku" },
        { startingYear: 1381, japaneseName: 'æ°¸å¾³', romanizedName: "Eitoku" },
        { startingYear: 1384, japaneseName: 'è³å¾³', romanizedName: "Shitoku" },
        { startingYear: 1387, japaneseName: 'åæ¶', romanizedName: "Kakei" },
        { startingYear: 1389, japaneseName: 'åº·å¿', romanizedName: "KÅÅ" },
        { startingYear: 1390, japaneseName: 'æå¾³', romanizedName: "Meitoku" },
        { startingYear: 1394, japaneseName: 'å¿æ°¸', romanizedName: "Åei" },
        { startingYear: 1428, japaneseName: 'æ­£é·', romanizedName: "ShÅchÅ" },
        { startingYear: 1429, japaneseName: 'æ°¸äº«', romanizedName: "EikyÅ" },
        { startingYear: 1441, japaneseName: 'åå', romanizedName: "Kakitsu" },
        { startingYear: 1444, japaneseName: 'æå®', romanizedName: "Bunnan" },
        { startingYear: 1449, japaneseName: 'å®å¾³', romanizedName: "HÅtoku" },
        { startingYear: 1452, japaneseName: 'äº«å¾³', romanizedName: "KyÅtoku" },
        { startingYear: 1455, japaneseName: 'åº·æ­£', romanizedName: "KÅshÅ" },
        { startingYear: 1457, japaneseName: 'é·ç¦', romanizedName: "ChÅroku" },
        { startingYear: 1460, japaneseName: 'å¯æ­£', romanizedName: "KanshÅ" },
        { startingYear: 1466, japaneseName: 'ææ­£', romanizedName: "BunshÅ" },
        { startingYear: 1467, japaneseName: 'å¿ä»', romanizedName: "Ånin" },
        { startingYear: 1469, japaneseName: 'ææ', romanizedName: "Bunmei" },
        { startingYear: 1487, japaneseName: 'é·äº«', romanizedName: "ChÅkyÅ" },
        { startingYear: 1489, japaneseName: 'å»¶å¾³', romanizedName: "Entoku" },
        { startingYear: 1492, japaneseName: 'æå¿', romanizedName: "MeiÅ" },
        { startingYear: 1501, japaneseName: 'æäº', romanizedName: "Bunki" },
        { startingYear: 1504, japaneseName: 'æ°¸æ­£', romanizedName: "EishÅ" },
        { startingYear: 1521, japaneseName: 'å¤§æ°¸', romanizedName: "Daiei" },
        { startingYear: 1528, japaneseName: 'äº«ç¦', romanizedName: "KyÅroku" },
        { startingYear: 1532, japaneseName: 'å¤©æ', romanizedName: "Tenbun" },
        { startingYear: 1555, japaneseName: 'å¼æ²»', romanizedName: "KÅji" },
        { startingYear: 1558, japaneseName: 'æ°¸ç¦', romanizedName: "Eiroku" },
        { startingYear: 1570, japaneseName: 'åäº', romanizedName: "Genki" },
        { startingYear: 1573, japaneseName: 'å¤©æ­£', romanizedName: "TenshÅ" },
        { startingYear: 1592, japaneseName: 'æç¦', romanizedName: "Bunroku" },
        { startingYear: 1596, japaneseName: 'æ¶é·', romanizedName: "KeichÅ" },
        { startingYear: 1615, japaneseName: 'åå', romanizedName: "Genna" },
        { startingYear: 1624, japaneseName: 'å¯æ°¸', romanizedName: "Kan'ei" },
        { startingYear: 1644, japaneseName: 'æ­£ä¿', romanizedName: "ShÅhÅ" },
        { startingYear: 1648, japaneseName: 'æ¶å®', romanizedName: "Keian" },
        { startingYear: 1652, japaneseName: 'æ¿å¿', romanizedName: "JÅÅ" },
        { startingYear: 1655, japaneseName: 'ææ¦', romanizedName: "Meireki" },
        { startingYear: 1658, japaneseName: 'ä¸æ²»', romanizedName: "Manji" },
        { startingYear: 1661, japaneseName: 'å¯æ', romanizedName: "Kanbun" },
        { startingYear: 1673, japaneseName: 'å»¶å®', romanizedName: "EnpÅ" },
        { startingYear: 1681, japaneseName: 'å¤©å', romanizedName: "Tenna" },
        { startingYear: 1684, japaneseName: 'è²äº«', romanizedName: "JÅkyÅ" },
        { startingYear: 1688, japaneseName: 'åç¦', romanizedName: "Genroku" },
        { startingYear: 1704, japaneseName: 'å®æ°¸', romanizedName: "HÅei" },
        { startingYear: 1711, japaneseName: 'æ­£å¾³', romanizedName: "ShÅtoku" },
        { startingYear: 1716, japaneseName: 'äº«ä¿', romanizedName: "KyÅhÅ" },
        { startingYear: 1736, japaneseName: 'åæ', romanizedName: "Genbun" },
        { startingYear: 1741, japaneseName: 'å¯ä¿', romanizedName: "KanpÅ" },
        { startingYear: 1744, japaneseName: 'å»¶äº«', romanizedName: "EnkyÅ" },
        { startingYear: 1748, japaneseName: 'å¯å»¶', romanizedName: "Kan'en" },
        { startingYear: 1751, japaneseName: 'å®æ¦', romanizedName: "HÅreki" },
        { startingYear: 1764, japaneseName: 'æå', romanizedName: "Meiwa" },
        { startingYear: 1772, japaneseName: 'å®æ°¸', romanizedName: "An'ei" },
        { startingYear: 1781, japaneseName: 'å¤©æ', romanizedName: "Tenmei" },
        { startingYear: 1789, japaneseName: 'å¯æ¿', romanizedName: "Kansei" },
        { startingYear: 1801, japaneseName: 'äº«å', romanizedName: "KyÅwa" },
        { startingYear: 1804, japaneseName: 'æå', romanizedName: "Bunka" },
        { startingYear: 1818, japaneseName: 'ææ¿', romanizedName: "Bunsei" },
        { startingYear: 1830, japaneseName: 'å¤©ä¿', romanizedName: "TenpÅ" },
        { startingYear: 1844, japaneseName: 'å¼å', romanizedName: "KÅka" },
        { startingYear: 1848, japaneseName: 'åæ°¸', romanizedName: "Kaei" },
        { startingYear: 1854, japaneseName: 'å®æ¿', romanizedName: "Ansei" },
        { startingYear: 1860, japaneseName: 'ä¸å»¶', romanizedName: "Man'en" },
        { startingYear: 1861, japaneseName: 'æä¹', romanizedName: "BunkyÅ«" },
        { startingYear: 1864, japaneseName: 'åæ²»', romanizedName: "Genji" },
        { startingYear: 1865, japaneseName: 'æ¶å¿', romanizedName: "KeiÅ" },
        { startingYear: 1868, japaneseName: 'ææ²»', romanizedName: "Meiji" },
        { startingYear: 1912, japaneseName: 'å¤§æ­£', romanizedName: "TaishÅ" },
        { startingYear: 1926, japaneseName: 'æ­å', romanizedName: "ShÅwa" },
        { startingYear: 1989, japaneseName: 'å¹³æ', romanizedName: "Heisei" }
    ]
);
