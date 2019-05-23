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

    @Test
    public void sub() {
        BigSignNum x = new BigSignNum(5);
        x.sub(new BigSignNum(3));
        assertEquals(x.toString(),"2");
        x = new BigSignNum(-3);
        x.sub(new BigSignNum(-5));
        assertEquals(x.toString(),"2");
        x = new BigSignNum(-5);
        x.sub(new BigSignNum(-3));
        assertEquals(x.toString(),"-2");
        x = new BigSignNum(3);
        x.sub(new BigSignNum(5));
        assertEquals(x.toString(),"-2");
        x = new BigSignNum(-5);
        x.sub(new BigSignNum(3));
        assertEquals(x.toString(),"-8");
        x = new BigSignNum(-3);
        x.sub(new BigSignNum(5));
        assertEquals(x.toString(),"-8");
        x = new BigSignNum(5);
        x.sub(new BigSignNum(-3));
        assertEquals(x.toString(),"8");
        x = new BigSignNum(3);
        x.sub(new BigSignNum(-5));
        assertEquals(x.toString(),"8");
}

    @Test
    public void mul() {
        BigSignNum x = new BigSignNum(5);
        x.mul(new BigSignNum(3));
        assertEquals(x.toString(),"15");
        x = new BigSignNum(-3);
        x.mul(new BigSignNum(-5));
        assertEquals(x.toString(),"15");
        x = new BigSignNum(-5);
        x.mul(new BigSignNum(3));
        assertEquals(x.toString(),"-15");
        x = new BigSignNum(-3);
        x.mul(new BigSignNum(5));
        assertEquals(x.toString(),"-15");
    }

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
