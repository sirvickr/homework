/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package task2;

import task1.BigNum;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author User
 */
public class BigSignNumTest {
    
    public BigSignNumTest() {
    }
    
    @BeforeClass
    public static void setUpClass() {
    }
    
    @AfterClass
    public static void tearDownClass() {
    }

    /**
     * Test of add method, of class BigSignNum.
     */
    @Test
    public void add() {
        BigSignNum x = new BigSignNum(2);
        x.add(new BigSignNum(2));
        assertEquals(x.toString(),"4");
        x.add(new BigSignNum(-8));
        assertEquals(x.toString(),"-4");
        x.add(new BigSignNum(2));
        assertEquals(x.toString(),"-2");
        x.add(new BigSignNum(-8));
        assertEquals(x.toString(),"-10");
    }

    /**
     * Test of sub method, of class BigSignNum.
     */
    @Test
    public void sub() {
        System.out.println("sub");
        fail("The test case is a prototype.");
    }

    /**
     * Test of mul method, of class BigSignNum.
     */
    @Test
    public void mul() {
        System.out.println("mul");
        fail("The test case is a prototype.");
    }

    /**
     * Test of toString method, of class BigSignNum.
     */
    @Test
    public void toStringTest() {
        assertEquals(new BigSignNum(10).toString(), "10");
        assertEquals(new BigSignNum(-9999).toString(), "-9999");
        assertEquals(new BigSignNum(-10203).toString(), "-10203");
        BigSignNum x = new BigSignNum(-11);
        long y = Integer.parseInt(x.toString());
        assertEquals(y, -11);
    }
    
}
