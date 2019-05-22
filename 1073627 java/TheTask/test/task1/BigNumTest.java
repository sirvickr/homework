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
    public void testFromLong() {
        System.out.println("fromLong");
        long x = 0L;
        BigNum instance = new BigNum();
        instance.fromLong(x);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    @Test
    public void testAdd() {
        /*
        System.out.println("add");
        BigNum x = null;
        BigNum instance = new BigNum();
        instance.add(x);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
        */
        BigNum x = new BigNum(55);
        x.add(x);
        assertEquals(x.toString(), "110");
        x.add(new BigNum(9999));
        assertEquals(x.toString(), "10109");
    }

    @Test
    public void testSub() {
        System.out.println("sub");
        BigNum x = null;
        BigNum instance = new BigNum();
        instance.sub(x);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    @Test
    public void testMul() {
        System.out.println("mul");
        BigNum x = null;
        BigNum instance = new BigNum();
        instance.mul(x);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    @Test
    public void testIsLarger() {
        System.out.println("isLarger");
        BigNum x = null;
        BigNum instance = new BigNum();
        boolean expResult = false;
        boolean result = instance.isLarger(x);
        assertEquals(expResult, result);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of shift method, of class BigNum.
     */
    @Test
    public void testShift() {
        System.out.println("shift");
        int x = 0;
        BigNum instance = new BigNum();
        instance.shift(x);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }

    /**
     * Test of toString method, of class BigNum.
     */
    @Test
    public void testToString() {
        System.out.println("toString");
        BigNum instance = new BigNum();
        String expResult = "";
        String result = instance.toString();
        assertEquals(expResult, result);
        // TODO review the generated test code and remove the default call to fail.
        fail("The test case is a prototype.");
    }
    
}
