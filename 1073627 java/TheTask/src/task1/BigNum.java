package task1;

/**
 * @author User
 */
public class BigNum {
    static final int base = 100;
    static final int len = 4;
    byte [] digits;
    public int digitsCount = 0;

    BigNum(){
        digits = new byte[len];
        digitsCount = 0;
    }

    BigNum(long x){
        this();
        fromLong(x);
    }

    BigNum(BigNum bigNum){
        digits = bigNum.digits.clone();
    }
    
    void fromLong(long x){
        digitsCount = 0;
        for(int i = 0; x > 0; i++){
            digits[i] = (byte) (x % base);
            x = x / base;
            digitsCount = digitsCount + 1;
        }
        //System.out.println("fromLong: " + this.toString() + " digitsCount " + this.digitsCount);
    }

    void add(BigNum x){
        //System.out.println("add: " + x.toString());
        byte c = 0;
        int sum;
        for(int i = 0; i < len; i++) {
            //byte temp1 = digits[i];
            //byte temp2 = c;
            sum = digits[i] + x.digits[i] + c;
            digits[i] = (byte) (sum % base);
            c = (byte) (sum / base);
            if(digits[i] > 0) {
                digitsCount = i + 1;
            }
            //System.out.println("digits[" + i + "] " + temp1 + " + " + x.digits[i] + " + " + temp2 + " = " + digits[i] + " (" + c + ") sum " + sum);
        }
        //System.out.println("add: " + this.toString() + " digitsCount " + this.digitsCount);
    }

    void sub(BigNum x){
        System.out.println("sub: " + x.toString());
        byte c = 0;
        int diff;
        for(int i = 0; i < len; i++) {
            byte temp1 = digits[i];
            byte temp2 = c;
            int digit = digits[i] - c;
            
            if(digit < x.digits[i]){
                c = 1;
                digit = digit + base;
            } else {
                c = 0;
            }
            
            diff = digit - x.digits[i];
            digits[i] = (byte) (diff/* % base*/);
            //c = (byte) (diff / base);
            if(digits[i] > 0) {
                digitsCount = i + 1;
            }
            System.out.println("digits[" + i + "] " + temp1 + " + " + x.digits[i] + " + " + temp2 + " = " + digits[i] + " (" + c + ") diff " + diff);
        }
        //System.out.println("sub: " + this.toString() + " digitsCount " + this.digitsCount);
    }

    void mul(BigNum x){
        System.out.println("mul: x " + x.toString() + " : " + x.digitsCount);
        BigNum result = new BigNum();
        for(int i = 0; i < x.digitsCount; i++) {
            byte c = 0;
            int digit = digits[i];
            BigNum y = this.mulDigit(x.digits[i], i);
            result.add(y);
            if(digits[i] > 0) {
                digitsCount = i + 1;
            }
            System.out.println(this.toString() + " * " + x.digits[i] + " = " + y.toString() + " result " + result.toString());
        }
        digits = result.digits;
        for(int i = x.len - 1; i >= 0; i--) {
            if (digits[i] > 0) {
                digitsCount = i + 1;
                break;
            }
        }
        System.out.println("mul: " + this.toString() + " digitsCount " + this.digitsCount);
    }

    boolean isLarger(BigNum x){
        for(int i = 0; i < len; i++) {
            if(digits[i] > x.digits[i]){
                return true;
            }
        }
        return false;
    }

    private BigNum mulDigit(byte digit, int shift) { // TODO
        //System.out.println("mulDigit: digit " + digit + " shift " + shift);
        BigNum x = new BigNum();
        byte c = 0;
        int m, j;
        for(int i = 0; i < len - shift; i++) {
            j = i + shift;
            //byte temp = c;
            m = digit * digits[i] + c;
            x.digits[j] = (byte) (m % base);
            c = (byte) (m / base);
            if(x.digits[j] > 0) {
                x.digitsCount = j + 1;
            }
            //System.out.println("digits[" + i + "] " + digit + " * " + digits[i] + " + " + temp + " = " + x.digits[i] + " (" + c + ") m " + m);
        }
        //System.out.println("mulDigit: " + x.toString() + " digitsCount " + x.digitsCount);
        return x;
    }

    void shift(int x){ // TODO
    }

    public String toString(){
        String result = "";
        int i = len - 1;
        while(i>0 & digits[i]==0)
            i--;
        while(i>=0) {
            String d = ((Byte) digits[i--]).toString();
            if(d.length()==1)
                d = "0" + d;
            result = result + d;
        }
        if(result.charAt(0)=='0')
            return result.substring(1);
        return result;
    }
    
    public static void main(String[] args) {
        {
            System.out.println("*************************************************");
            System.out.println("********************** add **********************");
            BigNum x = new BigNum(55);
            System.out.println("BigNum: " + x.toString());
            x.add(x);
            System.out.println("assertEquals(110):\t" + x.toString()); // 110
            x.add(new BigNum(9999));
            System.out.println("assertEquals(10109):\t" + x.toString()); // 110
        }
        {
            System.out.println("*************************************************");
            System.out.println("********************** sub **********************");
            BigNum x = new BigNum(25382457);
            System.out.println("BigNum: " + x.toString());
            x.sub(new BigNum(1101599));
            System.out.println("24280858: " + x.toString());
        }
        {
            System.out.println("*************************************************");
            System.out.println("**********************  >  **********************");
            BigNum x = new BigNum(25382457);
            BigNum y = new BigNum(25382457);
            System.out.println("BigNum: " + x.toString() + " > " + y.toString() + " " + x.isLarger(y));
        }
        {
            System.out.println("*************************************************");
            System.out.println("**********************  *  **********************");
            BigNum x = new BigNum(82457);
            byte digit = (byte)9;
            BigNum y = x.mulDigit(digit, 1);
            System.out.println("BigNum: " + x.toString() + " * " + digit + " = " + y.toString());
        }
        {
            System.out.println("*************************************************");
            System.out.println("********************** mul **********************");
            BigNum x = new BigNum(812);
            BigNum y = new BigNum(215);
            x.mul(y);
            System.out.println("BigNum: " + "812" + " * " + y.toString() + " = " + x.toString());
        }
    }
}
