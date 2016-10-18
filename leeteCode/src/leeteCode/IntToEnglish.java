package leeteCode;

public class IntToEnglish {
    static String[] to_19 = { "zero",  "one",   "two",  "three", "four",   "five",   "six",
        "seven", "eight", "nine", "ten",   "eleven", "twelve", "thirteen",
        "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" };
    static String[] tens  = { "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
    static String[] denom = { "",
        "thousand",     "million",         "billion",       "trillion",       "quadrillion",
        "quintillion",  "sextillion",      "septillion",    "octillion",      "nonillion",
        "decillion",    "undecillion",     "duodecillion",  "tredecillion",   "quattuordecillion",
        "sexdecillion", "septendecillion", "octodecillion", "novemdecillion", "vigintillion" };

    public static void main(String[] argv) throws Exception {
        int tstValue = 1999999;
        IntToEnglish itoe = new IntToEnglish();
        System.out.println(itoe.english_number(tstValue));
        /* for (int i = 0; i < 2147483647; i++) {
            System.out.println(itoe.english_number(i));
        } */
    }
    // convert a value < 100 to English.
    private String convert_nn(int val) throws Exception {
        if (val < 20)
            return to_19[val];
        for (int v = 0; v < tens.length; v++) {
            String dcap = tens[v];
            int dval = 20 + 10 * v;
            if (dval + 10 > val) {
                if ((val % 10) != 0)
                    return dcap + "-" + to_19[val % 10];
                return dcap;
            }        
        }
        throw new Exception("Should never get here, less than 100 failure");
    }
    // convert a value < 1000 to english, special cased because it is the level that kicks 
    // off the < 100 special case.  The rest are more general.  This also allows you to
    // get strings in the form of "forty-five hundred" if called directly.
    private String convert_nnn(int val) throws Exception {
        String word = "";
        int rem = val / 100;
        int mod = val % 100;
        if (rem > 0) {
            word = to_19[rem] + " hundred";
            if (mod > 0) {
                word = word + " ";
            }
        }
        if (mod > 0) {
            word = word + convert_nn(mod);
        }
        return word;
    }
    public String english_number(int val) throws Exception {
        if (val < 100) {
            return convert_nn(val);
        }
        if (val < 1000) {
            return convert_nnn(val);
        }
        for (int v = 0; v < denom.length; v++) {
            int didx = v - 1;
            int dval = new Double(Math.pow(1000, v)).intValue();
            if (dval > val) {
                int mod = new Double(Math.pow(1000, didx)).intValue();
                int l = val / mod;
                int r = val - (l * mod);
                String ret = convert_nnn(l) + " " + denom[didx];
                if (r > 0) {
                    ret = ret + ", " + english_number(r);
                }
                return ret;
            }
        }
        throw new Exception("Should never get here, bottomed out in english_number");
    }
}