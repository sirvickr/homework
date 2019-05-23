/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package task3;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author User
 */
public class BigFloatTest {
    
    public BigFloatTest() {
    }
    
    @BeforeClass
    public static void setUpClass() {
    }
    
    @AfterClass
    public static void tearDownClass() {
    }

    @Test
    public void normalize() {
        BigFloat x = new BigFloat(100);
        x.sub(new BigFloat(99.99f));
        x.normalize();
        float y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y - 0.01) < 0.001);
    }

    @Test
    public void add() {
        BigFloat x = new BigFloat(100);
        x.add(new BigFloat((float) 0.01));
        float y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y - 100.01) < 0.001);
        x = new BigFloat(1);
        x.add(new BigFloat((float) -10000));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y + 9999f) < 0.01);
    }

    @Test
    public void sub() {
        BigFloat x = new BigFloat(5);
        x.sub(new BigFloat(3));
        float y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y - 2.0f) < 0.001);
        x = new BigFloat(-3);
        x.sub(new BigFloat(-5));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y - 2.0f) < 0.001);
        x = new BigFloat(-5);
        x.sub(new BigFloat(-3));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y + 2.0f) < 0.001);
        x = new BigFloat(3);
        x.sub(new BigFloat(5));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y + 2.0f) < 0.001);
        x = new BigFloat(-5);
        x.sub(new BigFloat(3));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y + 8.0f) < 0.001);
        x = new BigFloat(-3);
        x.sub(new BigFloat(5));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y + 8.0f) < 0.001);
        x = new BigFloat(5);
        x.sub(new BigFloat(-3));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y - 8.0f) < 0.001);
        x = new BigFloat(3);
        x.sub(new BigFloat(-5));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y - 8.0f) < 0.001);
    }

    @Test
    public void mul() {
        BigFloat x = new BigFloat(5);
        x.mul(new BigFloat(3));
        float y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y - 15.0f) < 0.001);
        x = new BigFloat(-3);
        x.mul(new BigFloat(-5));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y - 15.0f) < 0.001);
        x = new BigFloat(-5);
        x.mul(new BigFloat(3));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y + 15.0f) < 0.001);
        x = new BigFloat(-3);
        x.mul(new BigFloat(5));
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y + 15.0f) < 0.001);
    }

    @Test
    public void toStringTest() {
        BigFloat x = new BigFloat((float) 1);
        float y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y - 1) < 0.001);
        x = new BigFloat((float) -10.01);
        y = Float.parseFloat(x.toString());
        assertTrue(Math.abs(y + 10.01) < 0.001);
    }

}
