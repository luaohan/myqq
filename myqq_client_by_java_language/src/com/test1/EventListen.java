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
 
    //���캯��
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

//��MyPanel֪����갴�µ���Ϣ��������֪�����λ�õ�����   MouseListener
//��MyPanel֪���ĸ���������  KeyListener
//��MyPanel֪������ƶ�����ק     MouseMotionListener
//��MyPanel֪�����ڵı仯���رա���󻯡���С����  WindowListener
 
class MyPanel extends JPanel implements WindowListener,MouseListener,KeyListener,MouseMotionListener
{
    public void paint (Graphics g)
    {
       super.paint(g);
   
    }
 
    //�����
    public void mouseClicked(MouseEvent e) { 
      
       System.out.println("���x="+e.getX()+"  "+"y="+e.getY());
    }
 
    //����ƶ���MyPanel
    public void mouseEntered(MouseEvent e) {
       System.out.println("������ˣ�");
    }
 
    //����뿪
    public void mouseExited(MouseEvent e) {
      
    }
 
    //��갴��
    public void mousePressed(MouseEvent e) {
      
    }
 
    //����ɿ�
    public void mouseReleased(MouseEvent e) {
      
    }
 
    //������ û���ɿ�
    public void keyPressed(KeyEvent e) {
       System.out.println(e.getKeyChar()+"��������");
      
    }
 
    //���ɿ�
    public void keyReleased(KeyEvent e) {
      
    }
 
    //�����룬Ҫ���о��������
    public void keyTyped(KeyEvent e) {
      
    }
 
    //�����ק
    public void mouseDragged(MouseEvent e) {
      
    }
 
    //����ƶ�
    public void mouseMoved(MouseEvent e) {
       System.out.println("���x="+e.getX()+"  "+"y="+e.getY());
      
    }
 
    //���ڼ�����
    public void windowActivated(WindowEvent e) {
       System.out.println("���ڼ�����");
 
    }
 
    //���ڹر���
    public void windowClosed(WindowEvent e) {
       System.out.println("���ڹر���");
    }
 
    //�������ڹر�
    public void windowClosing(WindowEvent e) {
       System.out.println("�������ڹر�");
    }
 
    //������С��
    public void windowDeactivated(WindowEvent e) {
      
    }
 
   
    public void windowDeiconified(WindowEvent e) {
      
    }
 
    public void windowIconified(WindowEvent e) {
      
    }
 
    //���ڴ���
    public void windowOpened(WindowEvent e) {
       System.out.println("���ڴ���");
    }
}
