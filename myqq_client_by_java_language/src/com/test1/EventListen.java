package com.test1;

//public class EventListen {
//
//	public EventListen() {
//		// TODO Auto-generated constructor stub
//	}
//
//	/**
//	 * @param args
//	 */
//	public static void main(String[] args) {
//		// TODO Auto-generated method stub
//
//	}
//
//}















import java.awt.*;
import javax.swing.*;
 
 
import java.awt.event.*;
 
public class EventListen extends JFrame
{
    MyPanel mp=null;
   
    public static void main(String[] args)
    {
//    	EventListen test=new EventListen();
    }
 
    //构造函数
    public EventListen()
    {
       mp=new MyPanel();
       this.addMouseListener(mp);
       this.addKeyListener(mp);
       this.addMouseMotionListener(mp);
       this.addWindowListener(mp);
       this.add(mp);
       this.setVisible(true);
       this.setSize(200, 300); 
      
    }
   
}

//让MyPanel知道鼠标按下的消息，并且能知道点击位置的坐标   MouseListener
//让MyPanel知道哪个键按下了  KeyListener
//让MyPanel知道鼠标移动，拖拽     MouseMotionListener
//让MyPanel知道窗口的变化（关闭、最大化、最小化）  WindowListener
 
class MyPanel extends JPanel implements WindowListener,MouseListener,KeyListener,MouseMotionListener
{
    public void paint (Graphics g)
    {
       super.paint(g);
   
    }
 
    //鼠标点击
    public void mouseClicked(MouseEvent e) { 
      
       System.out.println("鼠标x="+e.getX()+"  "+"y="+e.getY());
    }
 
    //鼠标移动到MyPanel
    public void mouseEntered(MouseEvent e) {
       System.out.println("鼠标来了！");
    }
 
    //鼠标离开
    public void mouseExited(MouseEvent e) {
      
    }
 
    //鼠标按下
    public void mousePressed(MouseEvent e) {
      
    }
 
    //鼠标松开
    public void mouseReleased(MouseEvent e) {
      
    }
 
    //键按下 没有松开
    public void keyPressed(KeyEvent e) {
       System.out.println(e.getKeyChar()+"键被按下");
      
    }
 
    //键松开
    public void keyReleased(KeyEvent e) {
      
    }
 
    //键输入，要求有具体的输入
    public void keyTyped(KeyEvent e) {
      
    }
 
    //鼠标拖拽
    public void mouseDragged(MouseEvent e) {
      
    }
 
    //鼠标移动
    public void mouseMoved(MouseEvent e) {
       System.out.println("鼠标x="+e.getX()+"  "+"y="+e.getY());
      
    }
 
    //窗口激活了
    public void windowActivated(WindowEvent e) {
       System.out.println("窗口激活了");
 
    }
 
    //窗口关闭了
    public void windowClosed(WindowEvent e) {
       System.out.println("窗口关闭了");
    }
 
    //窗口正在关闭
    public void windowClosing(WindowEvent e) {
       System.out.println("窗口正在关闭");
    }
 
    //窗口最小化
    public void windowDeactivated(WindowEvent e) {
      
    }
 
   
    public void windowDeiconified(WindowEvent e) {
      
    }
 
    public void windowIconified(WindowEvent e) {
      
    }
 
    //窗口打开了
    public void windowOpened(WindowEvent e) {
       System.out.println("窗口打开了");
    }
}
