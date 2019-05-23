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
        System.out.println("BigFloat: " + x + " log = " + Math.log10(1000000 / Math.abs(x)));
        shift = -(long) (Math.log10(1000000 / Math.abs(x)) / 2 + 0.5);
        //System.out.println(" shift " + shift);
        long xl = (long) (x * Math.pow(100, -shift) + 0.5);
        //System.out.println(" xl " + xl);
        if(x > 0)
            isPositive = true;
        else {
            xl = -xl;
            isPositive = false;
        }
        fromLong(xl);
        //System.out.println(" this " + toString());
        normalize();
        //System.out.println(" norm " + toString());
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
        //System.out.println("normalize: ");
        int i;
        for(i = len - 1; i >= 0 && digits[i]==0; i--)
            ;
        //System.out.println(" 1.shift " + shift + " " + super.toString());
        shift(len - i - 1);
        //System.out.println(" 2.shift " + shift + " " + super.toString());
        shift = shift - len + i + 1;
        //System.out.println(" 3.shift " + shift + " " + super.toString());
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
        System.out.println("sub: " + toString() + " + " + tmp.toString() + " shift " + shift + " " + tmp.shift + " isPositive " + isPositive + " tmp.isPositive " + tmp.isPositive);
        if(!isPositive && tmp.isPositive) {
            System.out.println("   -> add 1 ");
            isPositive = true;
            add(tmp);
            isPositive = false;
        } else if(isPositive && !tmp.isPositive) {
            System.out.println("   -> add 2 ");
            tmp.isPositive = true;
            add(tmp);
        } else {
            calcShift(tmp);
            System.out.println("   : " + toString() + " + " + tmp.toString() + " shift " + shift + " " + tmp.shift + " isLarger " + this.isLarger(tmp) + " " + tmp.isLarger(this));
            if(isPositive && tmp.isPositive && tmp.isLarger(this) || !isPositive && !tmp.isPositive && super.isLarger(tmp)) {
                System.out.println("   -> -(5-3) ");
                if(super.isLarger(tmp))
                    super.sub(tmp);
                else {
                    ((BigNum)tmp).sub(this);
                    digits = tmp.digits.clone();
                    shift = tmp.shift;
                }
                this.isPositive = false;
            } else if(isPositive && tmp.isPositive && super.isLarger(tmp) || !isPositive && !tmp.isPositive && tmp.isLarger(this)) {
                System.out.println("   ->  (5-3) ");
                if(super.isLarger(tmp))
                    super.sub(tmp);
                else {
                    ((BigNum)tmp).sub(this);
                    digits = tmp.digits.clone();
                    shift = tmp.shift;
                }
                this.isPositive = true;
            } else {
                System.out.println("   ->  ???? ");
            }
        }
    }

    void mul(BigFloat y){
        System.out.println("mul: " + toString() + " * " + y.toString() + " shift " + shift + " " + y.shift + " isPositive " + isPositive + " " + y.isPositive);
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
        System.out.println("    " + a + " * " + b + " = " + x + ": " + toString());
    }

    public String toString(){
        String sign = isPositive ? "" : "-";
        return sign + super.toString() + "e" + ((Long)(shift * 2)).toString();
    }
    
    public static void main(String[] args) {
        //BigFloat y = new BigFloat(123.14f);
        /*{
            System.out.println("*************************************************");
            System.out.println("********************** norm *********************");
            BigFloat x = new BigFloat(100);
            x.sub(new BigFloat(99.99f));
            x.normalize();
            float y = Float.parseFloat(x.toString());
            System.out.println(" y " + y + " diff " + Math.abs(y - 0.01) + " (~0)");
            //assertTrue(Math.abs(y - 0.01) < 0.001);
        }*/
        /*{
            System.out.println("*************************************************");
            System.out.println("********************** add **********************");
            BigFloat x = new BigFloat(100);
            x.add(new BigFloat((float) 0.03));
            float y = Float.parseFloat(x.toString());
            System.out.println(" y " + y + " diff " + Math.abs(y - 100.01) + " (~0)");
            //assertTrue(Math.abs(y - 100.01) < 0.001);
            x = new BigFloat(1);
            x.add(new BigFloat((float) -10000));
            y = Float.parseFloat(x.toString());
            System.out.println(" y " + y + " diff " + Math.abs(y + 9999f) + " (~0)");
            //assertTrue(Math.abs(y + 9999f) < 0.001);
            x = new BigFloat(5);
            x.add(new BigFloat((float) -3));
            y = Float.parseFloat(x.toString());
            System.out.println(" y " + y + " diff " + Math.abs(y - 2f) + " (~0)");
        }*/
        /*{
            System.out.println("*************************************************");
            System.out.println("********************** sub **********************");
            BigFloat x = new BigFloat(5);
            x.sub(new BigFloat(3));
            System.out.println(" = " + x.toString() + " (2)");
            x = new BigFloat(-3);
            x.sub(new BigFloat(-5));
            System.out.println(" = " + x.toString() + " (2)");
            x = new BigFloat(-5);
            x.sub(new BigFloat(-3));
            System.out.println(" = " + x.toString() + " (-2)");
            x = new BigFloat(3);
            x.sub(new BigFloat(5));
            System.out.println(" = " + x.toString() + " (-2)");
            x = new BigFloat(-5);
            x.sub(new BigFloat(3));
            System.out.println(" = " + x.toString() + " (-8)");
            x = new BigFloat(-3);
            x.sub(new BigFloat(5));
            System.out.println(" = " + x.toString() + " (-8)");
            x = new BigFloat(5);
            x.sub(new BigFloat(-3));
            System.out.println(" = " + x.toString() + " (8)");
            x = new BigFloat(3);
            x.sub(new BigFloat(-5));
            System.out.println(" = " + x.toString() + " (8)");
        }*/
        {
            System.out.println("*************************************************");
            System.out.println("********************** mul **********************");
            BigFloat x = new BigFloat(5);
            x.mul(new BigFloat(3));
            float y = Float.parseFloat(x.toString());
            System.out.println(" = " + y + " " + x.toString() + " (15)");
            x = new BigFloat(-3);
            x.mul(new BigFloat(-5));
            System.out.println(" = " + x.toString() + " (15)");
            x = new BigFloat(-5);
            x.mul(new BigFloat(3));
            System.out.println(" = " + x.toString() + " (-15)");
            x = new BigFloat(-3);
            x.mul(new BigFloat(5));
            System.out.println(" = " + x.toString() + " (-15)");
        }
    }
}
