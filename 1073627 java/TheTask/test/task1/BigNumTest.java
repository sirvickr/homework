package task1;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 * @author User
 */

public class BigNumTest {
    
    public BigNumTest() {
    }
    
    @BeforeClass
    public static void setUpClass() {
    }
    
    @AfterClass
    public static void tearDownClass() {
    }

    @Test
    public void fromLong() {
        BigNum x = new BigNum(12345678);
        assertEquals(x.toString(), "12345678");
    }

    @Test
    public void add() {
        BigNum x = new BigNum(55);
        x.add(x);
        assertEquals(x.toString(), "110");
        x.add(new BigNum(9999));
        assertEquals(x.toString(), "10109");
    }

    @Test
    public void sub() {
        BigNum x = new BigNum(25382457);
        x.sub(new BigNum(1101599));
        assertEquals(x.toString(), "24280858");
        x.sub(new BigNum(95612));
        assertEquals(x.toString(), "24185246");
    }

    @Test
    public void mul() {
        BigNum x = new BigNum(812);
        BigNum y = new BigNum(215);
        x.mul(y);
        assertEquals(x.toString(), "174580");
    }

    @Test
    public void isLarger() {
        BigNum x = new BigNum(550);
        assertEquals(x.isLarger(new BigNum(549)), true);
        assertEquals(x.isLarger(new BigNum(550)), false);
        assertEquals(x.isLarger(new BigNum(551)), false);
    }

    @Test
    public void shift() {
        BigNum x = new BigNum(812);
        x.shift(2);
        assertEquals(x.toString(), "8120000");
        x.shift(-1);
        assertEquals(x.toString(), "81200");
        x.shift(1);
        assertEquals(x.toString(), "8120000");
        x.shift(-2);
        assertEquals(x.toString(), "812");
    }

    @Test
    public void toStringTest() {
        assertEquals(new BigNum(10).toString(), "10");
        assertEquals(new BigNum(9999).toString(), "9999");
        assertEquals(new BigNum(10203).toString(), "10203");
        BigNum x = new BigNum(1);
        long y = Integer.parseInt(x.toString());
        assertEquals(y, 1);
    }
    
}
