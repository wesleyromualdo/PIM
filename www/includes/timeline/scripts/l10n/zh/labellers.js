ï»¿/*==================================================
 *  Localization of labellers.js
 *==================================================
 */

Timeline.GregorianDateLabeller.monthNames["zh"] = [
    "1æ", "2æ", "3æ", "4æ", "5æ", "6æ", "7æ", "8æ", "9æ", "10æ", "11æ", "12æ"
];

Timeline.GregorianDateLabeller.labelIntervalFunctions["zh"] = function(date, intervalUnit) {
    var text;
    var emphasized = false;
    
    var date2 = Timeline.DateTime.removeTimeZoneOffset(date, this._timeZone);
    
    switch(intervalUnit) {
    case Timeline.DateTime.DAY:
    case Timeline.DateTime.WEEK:
        text = Timeline.GregorianDateLabeller.getMonthName(date2.getUTCMonth(), this._locale) + 
            date2.getUTCDate() + "æ¥";
        break;
    default:
        return this.defaultLabelInterval(date, intervalUnit);
    }
    
    return { text: text, emphasized: emphasized };
};