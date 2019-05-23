package task2;

import task1.BigNum;

/**
 * @author User
 */

public class BigSignNum {
    private BigNum num;
    private boolean isPositive;

    BigSignNum(long x) {
        if(x > 0) {
            num = new BigNum(x);
            isPositive = true;
        } else {
            num = new BigNum(-x);
            isPositive = false;
        }
    }

    BigSignNum(BigSignNum x) {
        num = new BigNum(x.num);
        isPositive = x.isPositive;
    }

    void add(BigSignNum x){
        if(x.isPositive)
            if(isPositive)
                num.add(x.num);
            else
                if(num.isLarger(x.num))
                    num.sub(x.num);
                else {
                    BigNum tmp = new BigNum(x.num);
                    tmp.sub(num);
                    num = new BigNum(tmp);
                    isPositive = false;
                }
        else {
            BigSignNum tmp = new BigSignNum(x);
            tmp.isPositive = true;
            sub(tmp);
        }
    }

    void sub(BigSignNum x){
        if(this.num.isLarger(x.num) && isPositive && x.isPositive || x.num.isLarger(this.num) && !isPositive && !x.isPositive) {
            if(this.num.isLarger(x.num))
                this.num.sub(x.num);
            else {
                BigSignNum tmp = new BigSignNum(this);
                this.num = new BigNum(x.num);
                this.num.sub(tmp.num);
            }
            this.isPositive = true;
        } else if(x.num.isLarger(this.num) && isPositive && x.isPositive || this.num.isLarger(x.num) && !isPositive && !x.isPositive) {
            if(this.num.isLarger(x.num))
                this.num.sub(x.num);
            else {
                BigSignNum tmp = new BigSignNum(this);
                this.num = new BigNum(x.num);
                this.num.sub(tmp.num);
            }
            this.isPositive = false;
        } else if(this.num.isLarger(x.num) && !isPositive && x.isPositive || x.num.isLarger(this.num) && !isPositive && x.isPositive) {
            this.num.add(x.num);
            this.isPositive = false;
        } else if(this.num.isLarger(x.num) && isPositive && !x.isPositive || x.num.isLarger(this.num) && isPositive && !x.isPositive) {
            this.num.add(x.num);
            this.isPositive = true;
        }
    }

    void mul(BigSignNum x){
        this.num.mul(x.num);
        this.isPositive = (this.isPositive == x.isPositive);
    }

    public String toString(){
        if(isPositive)
            return num.toString();
        else
            return "-"+num.toString();
    }
    
    public static void main(String[] args) {
        {
            System.out.println("*************************************************");
            System.out.println("********************** add **********************");
            BigSignNum x = new BigSignNum(2);
            x.add(new BigSignNum(2));
            System.out.println(" = " + x.toString() + " (4)");
            x.add(new BigSignNum(-8));
            System.out.println(" = " + x.toString() + " (-4)");
            x.add(new BigSignNum(2));
            System.out.println(" = " + x.toString() + " (-2)");
            x.add(new BigSignNum(-8));
            System.out.println(" = " + x.toString() + " (-10)");
        }
        {
            System.out.println("*************************************************");
            System.out.println("********************** sub **********************");
            BigSignNum x = new BigSignNum(5);
            x.sub(new BigSignNum(3));
            System.out.println(" = " + x.toString() + " (2)");
            x = new BigSignNum(-3);
            x.sub(new BigSignNum(-5));
            System.out.println(" = " + x.toString() + " (2)");
            x = new BigSignNum(-5);
            x.sub(new BigSignNum(-3));
            System.out.println(" = " + x.toString() + " (-2)");
            x = new BigSignNum(3);
            x.sub(new BigSignNum(5));
            System.out.println(" = " + x.toString() + " (-2)");
            x = new BigSignNum(-5);
            x.sub(new BigSignNum(3));
            System.out.println(" = " + x.toString() + " (-8)");
            x = new BigSignNum(-3);
            x.sub(new BigSignNum(5));
            System.out.println(" = " + x.toString() + " (-8)");
            x = new BigSignNum(5);
            x.sub(new BigSignNum(-3));
            System.out.println(" = " + x.toString() + " (8)");
            x = new BigSignNum(3);
            x.sub(new BigSignNum(-5));
            System.out.println(" = " + x.toString() + " (8)");
        }
        {
            System.out.println("*************************************************");
            System.out.println("********************** mul **********************");
            BigSignNum x = new BigSignNum(5);
            x.mul(new BigSignNum(3));
            System.out.println(" = " + x.toString() + " (15)");
            x = new BigSignNum(-3);
            x.mul(new BigSignNum(-5));
            System.out.println(" = " + x.toString() + " (15)");
            x = new BigSignNum(-5);
            x.mul(new BigSignNum(3));
            System.out.println(" = " + x.toString() + " (-15)");
            x = new BigSignNum(-3);
            x.mul(new BigSignNum(5));
            System.out.println(" = " + x.toString() + " (-15)");
        }
    }
}
