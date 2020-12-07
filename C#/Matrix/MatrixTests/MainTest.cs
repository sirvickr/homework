using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ConsoleApplicationMatrix;

namespace MatrixTests
{
    [TestClass]
    public class MainTest
    {
        [TestMethod]
        [ExpectedException(typeof(MyException))]
        public void Matrix_Expected_MyException_i()
        {
            //act (выполнить)
            Matrix a = new Matrix(0, 2);
        }
        [TestMethod]
        [ExpectedException(typeof(MyException))]
        public void Matrix_Expected_MyException_j()
        {
            //act (выполнить)
            Matrix a = new Matrix(2, -1);
        }
        [TestMethod]
        [ExpectedException(typeof(MyException))]
        public void Sum_Expected_MyException()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 1);
            a[0, 0] = 1;
            a[1, 0] = 1;
            Matrix b = new Matrix(2, 2);
            b[0, 0] = 2;
            b[0, 1] = 2;
            b[1, 0] = 2;
            b[1, 1] = 2;
            //act (выполнить)
            Matrix c = a + b;
        }
        [TestMethod]
        public void Sum()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 2);
            a[0, 0] = 1;
            a[0, 1] = 1;
            a[1, 0] = 1;
            a[1, 1] = 1;
            Matrix b = new Matrix(2, 2);
            b[0, 0] = 2;
            b[0, 1] = 2;
            b[1, 0] = 2;
            b[1, 1] = 2;
            Matrix expected = new Matrix(2, 2);
            expected[0, 0] = 3;
            expected[0, 1] = 3;
            expected[1, 0] = 3;
            expected[1, 1] = 3;
            //act (выполнить)
            Matrix actual = a + b;
            //assert(доказать) 
            Assert.IsTrue(actual == expected); //Оракул 
        }
        [TestMethod]
        [ExpectedException(typeof(MyException))]
        public void Subtract_Expected_MyException()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 1);
            a[0, 0] = 1;
            a[1, 0] = 1;
            Matrix b = new Matrix(2, 2);
            b[0, 0] = 2;
            b[0, 1] = 2;
            b[1, 0] = 2;
            b[1, 1] = 2;
            //act (выполнить)
            Matrix c = a + b;
        }
        [TestMethod]
        public void Subtract()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 2);
            a[0, 0] = 3;
            a[0, 1] = 4;
            a[1, 0] = 5;
            a[1, 1] = 1;
            Matrix b = new Matrix(2, 2);
            b[0, 0] = 2;
            b[0, 1] = 2;
            b[1, 0] = 2;
            b[1, 1] = 2;
            Matrix expected = new Matrix(2, 2);
            expected[0, 0] = 1;
            expected[0, 1] = 2;
            expected[1, 0] = 3;
            expected[1, 1] = -1;
            //act (выполнить)
            Matrix actual = a - b;
            //assert(доказать) 
            Assert.IsTrue(actual == expected); //Оракул 
        }
        [TestMethod]
        [ExpectedException(typeof(MyException))]
        public void Multiply_Expected_MyException()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 1);
            a[0, 0] = 1;
            a[1, 0] = 1;
            Matrix b = new Matrix(2, 1);
            b[0, 0] = 2;
            b[1, 0] = 2;
            //act (выполнить)
            Matrix c = a * b;
        }
        [TestMethod]
        public void Multiply()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 3);
            a[0, 0] = 2;
            a[0, 1] = 3;
            a[0, 2] = 4;
            a[1, 0] = 5;
            a[1, 1] = 6;
            a[1, 2] = 7;
            Matrix b = new Matrix(3, 4);
            b[0, 0] = 2;
            b[0, 1] = 3;
            b[0, 2] = 4;
            b[0, 3] = 5;
            b[1, 0] = 6;
            b[1, 1] = 7;
            b[1, 2] = 8;
            b[1, 3] = 9;
            b[2, 0] = 10;
            b[2, 1] = 11;
            b[2, 2] = 12;
            b[2, 3] = 13;
            Matrix expected = new Matrix(2, 4);
            expected[0, 0] = 62;
            expected[0, 1] = 71;
            expected[0, 2] = 80;
            expected[0, 3] = 89;
            expected[1, 0] = 116;
            expected[1, 1] = 134;
            expected[1, 2] = 152;
            expected[1, 3] = 170;
            //act (выполнить)
            Matrix actual = a * b;
            //assert(доказать) 
            Assert.IsTrue(actual == expected); //Оракул 
        }
        [TestMethod]
        public void Equal()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 2);
            a[0, 0] = 1;
            a[0, 1] = 1;
            a[1, 0] = 1;
            a[1, 1] = 1;
            Matrix b = new Matrix(2, 2);
            b[0, 0] = 1;
            b[0, 1] = 1;
            b[1, 0] = 1;
            b[1, 1] = 1;
            //act (выполнить) 
            bool r = a == b;
            //assert(доказать) 
            Assert.IsTrue(r);
            Assert.AreEqual(a, b);
        }
        [TestMethod]
        [ExpectedException(typeof(MyException))]
        public void Transp_Expected_MyException()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 3);
            a[0, 0] = 2;
            a[0, 1] = 3;
            a[0, 2] = 4;
            a[1, 0] = 5;
            a[1, 1] = 6;
            a[1, 2] = 7;
            //act (выполнить)
            Matrix b = a.Transp();
        }
        [TestMethod]
        public void Transp()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 2);
            a[0, 0] = 2;
            a[0, 1] = 3;
            a[1, 0] = 4;
            a[1, 1] = 5;
            Matrix expected = new Matrix(2, 2);
            expected[0, 0] = 2;
            expected[0, 1] = 4;
            expected[1, 0] = 3;
            expected[1, 1] = 5;
            //act (выполнить)
            Matrix actual = a.Transp();
            //assert(доказать) 
            Assert.IsTrue(actual == expected); //Оракул 
        }
        [TestMethod]
        public void Min()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 2);
            a[0, 0] = 2;
            a[0, 1] = 3;
            a[1, 0] = -1;
            a[1, 1] = 5;
            //act (выполнить)
            int min = a.Min();
            //assert(доказать) 
            Assert.IsTrue(min == -1); //Оракул 
        }
        [TestMethod]
        public void Str()
        {
            //arrange(обеспечить) 
            Matrix a = new Matrix(2, 2);
            a[0, 0] = 2;
            a[0, 1] = 3;
            a[1, 0] = -1;
            a[1, 1] = 5;
            string expected = "{{2,3},{-1,5}}";
            //act (выполнить)
            string actual = a.ToString();
            //assert(доказать) 
            Assert.IsTrue(expected == actual);
        }
        [TestMethod]
        [ExpectedException(typeof(MyException))]
        public void This_Expected_MyException_set_j()
        {
            //act (выполнить) 
            Matrix a = new Matrix(2, 2);
            a[1, 3] = 2;
        }
        [TestMethod]
        [ExpectedException(typeof(MyException))]
        public void This_Expected_MyException_get_i()
        {
            //act (выполнить) 
            Matrix a = new Matrix(2, 2);
            int r = a[3, 1];
        }
    }
}
