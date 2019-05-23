package task3;

import task1.BigNum;

/**
 * @author User
 */

public class BigFloat extends BigNum {
    private boolean isPositive;
    private long shift;

    BigFloat(float x){
        super();
        shift = -(long) (Math.log10(1000000 / Math.abs(x)) / 2 + 0.5);
        long xl = (long) (x * Math.pow(100, -shift) + 0.5);
        if(x > 0)
            isPositive = true;
        else {
            xl = -xl;
            isPositive = false;
        }
        fromLong(xl);
        normalize();
    }

    BigFloat(BigFloat x){
        super(x);
        isPositive = x.isPositive;
        shift = x.shift;
    }

    private void calcShift(BigFloat tmp){
        if(shift < tmp.shift)
            shift((int) (shift - tmp.shift));
        else
            tmp.shift((int) (tmp.shift - shift));
        shift = Math.max(shift, tmp.shift);
        tmp.shift = shift;
    }

    void normalize(){
        int i;
        for(i = len - 1; i >= 0 && digits[i]==0; i--)
            ;
        shift(len - i - 1);
        shift = shift - len + i + 1;
   }

    void add(BigFloat x) {
        BigFloat tmp = new BigFloat(x);
        //System.out.println("add: " + toString() + " + " + tmp.toString() + " shift " + shift + " " + tmp.shift);
        if (tmp.isPositive) {
            calcShift(tmp);
            //System.out.println("     " + toString() + " + " + tmp.toString() + " shift " + shift + " " + tmp.shift);
            if (isPositive) {
                //System.out.println("     ++");
                super.add(tmp);
            } else if (super.isLarger(tmp)) {
                //System.out.println("     ->");
                super.sub(tmp);
            } else {
                //System.out.println("     -<");
                ((BigNum)tmp).sub(this);
                digits = tmp.digits.clone();
                shift = tmp.shift;
                isPositive = false;
            }
            normalize();
        } else {
            tmp.isPositive = true;
            sub(tmp);
        }
    }

    void sub(BigFloat x) {
        BigFloat tmp = new BigFloat(x);
        if(!isPositive && tmp.isPositive) {
            isPositive = true;
            add(tmp);
            isPositive = false;
        } else if(isPositive && !tmp.isPositive) {
            tmp.isPositive = true;
            add(tmp);
        } else {
            calcShift(tmp);
            if(isPositive && tmp.isPositive && tmp.isLarger(this) || !isPositive && !tmp.isPositive && super.isLarger(tmp)) {
                if(super.isLarger(tmp))
                    super.sub(tmp);
                else {
                    ((BigNum)tmp).sub(this);
                    digits = tmp.digits.clone();
                    shift = tmp.shift;
                }
                this.isPositive = false;
            } else if(isPositive && tmp.isPositive && super.isLarger(tmp) || !isPositive && !tmp.isPositive && tmp.isLarger(this)) {
                if(super.isLarger(tmp))
                    super.sub(tmp);
                else {
                    ((BigNum)tmp).sub(this);
                    digits = tmp.digits.clone();
                    shift = tmp.shift;
                }
                this.isPositive = true;
            }
        }
    }

    void mul(BigFloat y){
        float a = Float.parseFloat(toString());
        float b = Float.parseFloat(y.toString());
        float x = a * b;
        digits = new byte[len];
        digitsCount = 0;
        shift = -(long) (Math.log10(1000000 / Math.abs(x)) / 2 + 0.5);
        long xl = (long) (x * Math.pow(100, -shift) + 0.5);
        if(x > 0)
            isPositive = true;
        else {
            xl = -xl;
            isPositive = false;
        }
        fromLong(xl);
        normalize();
    }

    public String toString(){
        String sign = isPositive ? "" : "-";
        return sign + super.toString() + "e" + ((Long)(shift * 2)).toString();
    }
}
