package task1;

/**
 * @author User
 */
public class BigNum {
    static final protected int base = 100;
    static final protected int len = 4;
    protected byte [] digits;
    protected int digitsCount = 0;

    public BigNum(){
        digits = new byte[len];
        digitsCount = 0;
    }

    public BigNum(long x){
        this();
        fromLong(x);
    }

    public BigNum(BigNum bigNum){
        digits = bigNum.digits.clone();
        digitsCount = bigNum.digitsCount;
    }

    protected void fromLong(long x){
        digitsCount = 0;
        for(int i = 0; x > 0; i++){
            digits[i] = (byte) (x % base);
            x = x / base;
            digitsCount = digitsCount + 1;
        }
    }

    public void add(BigNum x){
        byte c = 0;
        int sum;
        for(int i = 0; i < len; i++) {
            sum = digits[i] + x.digits[i] + c;
            digits[i] = (byte) (sum % base);
            c = (byte) (sum / base);
            if(digits[i] > 0) {
                digitsCount = i + 1;
            }
        }
    }

    public void sub(BigNum x){
        byte c = 0;
        int diff;
        for(int i = 0; i < len; i++) {
            int digit = digits[i] - c;
            if(digit < x.digits[i]){
                c = 1;
                digit = digit + base;
            } else {
                c = 0;
            }
            diff = digit - x.digits[i];
            digits[i] = (byte) (diff/* % base*/);
            if(digits[i] > 0) {
                digitsCount = i + 1;
            }
        }
    }

    public void mul(BigNum x){
        BigNum result = new BigNum();
        for(int i = 0; i < x.digitsCount; i++) {
            byte c = 0;
            int digit = digits[i];
            BigNum y = this.mulDigit(x.digits[i], i);
            result.add(y);
            if(digits[i] > 0) {
                digitsCount = i + 1;
            }
        }
        digits = result.digits;
        for(int i = len - 1; i >= 0; i--) {
            if (digits[i] > 0) {
                digitsCount = i + 1;
                break;
            }
        }
    }

    public boolean isLarger(BigNum x){
        for(int i = len - 1; i > 0; i--) {
            if(digits[i] > x.digits[i]){
                return true;
            } else if(x.digits[i] > digits[i]) {
                return false;
            }
        }
        return false;
    }

    private BigNum mulDigit(byte digit, int shift) { // TODO
        BigNum x = new BigNum();
        byte c = 0;
        int m, j;
        for(int i = 0; i < len - shift; i++) {
            j = i + shift;
            m = digit * digits[i] + c;
            x.digits[j] = (byte) (m % base);
            c = (byte) (m / base);
            if(x.digits[j] > 0) {
                x.digitsCount = j + 1;
            }
        }
        return x;
    }

    protected void shift(int x){
        int i;
        if(x > 0) { // сдвиг влево
            for(i = len - 1; i >= x; i--) {
                digits[i] = digits[i - x];
            }
            for(i = 0; i < x; i++) {
                digits[i] = 0;
            }
        } else if(x < 0) { // сдвиг вправо
            for(i = 0; i < len + x; i++) {
                digits[i] = digits[i - x];
            }
            for(int j = i; j < len; j++) {
                digits[j] = 0;
            }
        }
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
}
