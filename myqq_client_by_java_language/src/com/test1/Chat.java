package com.test1;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;

public class Chat extends JFrame implements ActionListener,MouseListener{

	JSplitPane jspp = null;  //JSplitPane ���ڷָ�������ֻ��������Component(jp_right,jp_left)
	
	JPanel jp_right = null;  //�ұߵ�jpanel
	JPanel jp_left = null;   //��ߵ�jpanel
	
	JLabel[] jlabel;         //�зź��ѣ���ߵĺ����б�
	
	JTextArea jta_show_info = null;    //��ʾ��Ϣ
	JTextArea jta_send_info = null;    //����������Ϣ
	
	JScrollPane jsp_jta_show_info = null;  //�з�jta_show_info
	JScrollPane jsp_jta_send_info = null;  //�з�jta_send_info
	
	
	JPanel jp_for_button = null;      //�зŷ��Ͱ�ť
	
	JButton button_send = null;       //���Ͱ�ť
	
	Protocol p = null;   //Э�����
	
	String username = null;
	
	String chattofriendname = null;
	
	String[] friendnames;
	
	public Chat(String[] friendnames, Protocol p, String username){
		
		this.friendnames = friendnames;
		this.username = username;
		this.p = p;
		
		jta_show_info = new JTextArea(13, 5);
		jta_show_info.setEnabled(false);      //��ֹ�༭
		
		jta_send_info = new JTextArea(5, 5);
		
		jsp_jta_show_info = new JScrollPane(jta_show_info);
		jsp_jta_send_info = new JScrollPane(jta_send_info);
		
		jp_for_button = new JPanel();
		
		jp_right = new JPanel(new BorderLayout());
		jp_left = new JPanel(new GridLayout(friendnames.length, 1));

		jlabel = new JLabel[friendnames.length];
		
		for(int i = 0; i < jlabel.length; i++){   
			jlabel[i] = new JLabel(friendnames[i],(new ImageIcon("image/qq.png")),JLabel.LEFT);
			jlabel[i].addMouseListener(this);       //����˫���¼�
			jp_left.add(jlabel[i]);
			
		}
		
		//����һ���߳����������Ϣ
		//�����ڳ�ʼ��jlabel[] ����֮����Ϊ��ʱ���ı��ǩ����ɫ
		ReadInfo thread =new ReadInfo(p);
		thread.start();
				
		button_send = new JButton("�������");
		button_send.addActionListener(this);       //��������¼�
		
		jp_for_button.add(button_send);
		
		jp_right.add(jsp_jta_show_info,BorderLayout.NORTH);
		jp_right.add(jsp_jta_send_info);
		jp_right.add(jp_for_button, BorderLayout.SOUTH);
		
		jspp = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,jp_left,jp_right);
		//ʹ��������jpanel �����۵�
		jspp.setOneTouchExpandable(true);  
		
		this.add(jspp);
		this.setTitle(username + "��������");
		this.setIconImage((new ImageIcon("image/qq.png")).getImage());
		this.setResizable(false);  //�����Ըı䴰�ڵĴ�С
		this.setSize(600, 400);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setLocation(300, 200);
		this.setVisible(true);
	}
	

	public void actionPerformed(ActionEvent e) {
		
		if (e.getSource() == button_send){  //�������
			if (chattofriendname != null){  //˫���˺���
				
				//�������Ƿ����ߣ���ɫ�������ߣ�����������
				for (int i = 0; i < friendnames.length; i++){
					if (jlabel[i].getText().equals(chattofriendname)){
						if (jlabel[i].getForeground() == Color.black){
							JOptionPane.showMessageDialog(this, "sorry, your friend is not online");
							return ;
						}
					}
				}
				//��������˵�����ߣ����Է�����Ϣ
				String info;
				try {
					info = new String(jta_send_info.getText());
					if (info.equals("")){
						JOptionPane.showMessageDialog(this, "please input information");
						return ;
					}
					
					
					
					int friendname_len = chattofriendname.getBytes().length;
					String s_name_len = String.valueOf(friendname_len);
					
					jta_show_info.append(username+":\r\n   "+info+"\r\n\r\n");
//					jta_show_info.add(new JLabel(username+":\r\n   "+info+"\r\n\r\n"));
					
					if(friendname_len < 10){
						info=("000"+s_name_len)+chattofriendname+info;
					}else if (friendname_len < 100){
						info=("00"+s_name_len)+chattofriendname+info;
					}else if(friendname_len < 1000){
						info=("0"+s_name_len)+chattofriendname+info;
					}
					
					p.mes = info;
					p.mes_type = Protocol.ChatMess;
					p.send_packet();
					
					
					jta_send_info.setText("");
					
					
				} catch (Exception e1) {
					e1.printStackTrace();
				}
			}else{
				JOptionPane.showMessageDialog(this, "Who do you want to Chat ?");
			}
		}
	}

	
	public void mouseClicked(MouseEvent e) {
		if (e.getClickCount() == 2){
			//�õ���������
			chattofriendname = ((JLabel)e.getSource()).getText();
			this.setTitle(username + " ������  "+chattofriendname+" ����");
		}
	}
	
	public void mousePressed(MouseEvent e) {
		
	}
	public void mouseReleased(MouseEvent e) {
		
	}
	public void mouseEntered(MouseEvent e) {
		
	}
	public void mouseExited(MouseEvent e) {
		
	}

	
class ReadInfo extends Thread{  //����ฺ�������Ϣ
		
		Protocol p = null;
		
		public ReadInfo(Protocol p){
			this.p = p;
		}
		
		public void run(){
			
			while(true){
				//����������Ϣ
				p.recv_packet();
				
				if (p.mes_type == p.FriendsStates){
					
					String[] friendstates = p.mes.split(" ");
					//���ݺ��ѵ�״̬���ú�����������ɫ
					for (int i = 0; i < friendnames.length; i++){
						if (friendstates[i].equals("1")){
							jlabel[i].setForeground(Color.green);
							jlabel[i].setEnabled(true);
						}else if (friendstates[i].equals("0")){
							jlabel[i].setForeground(Color.black);
							jlabel[i].setEnabled(false);
						}
					}
					
				}else if (p.mes_type == p.ON){   //��������
					//�ҵ�������ѣ���������ɫ
					for (int i = 0; i < friendnames.length; i++){
						if (jlabel[i].getText().equals(p.mes)){
							jlabel[i].setForeground(Color.green);
							jlabel[i].setEnabled(true);
							break;
						}
					}
					
				}else if(p.mes_type == p.OFF){   //��������
					//�ҵ�������ѣ���������ɫ
					for (int i = 0; i < friendnames.length; i++){
						if (jlabel[i].getText().equals(p.mes)){
							jlabel[i].setForeground(Color.black);
							jlabel[i].setEnabled(false);
							break;
						}
					}
					
				}else{ //������Ϊ������Ϣ
					
//					jta_show_info.setText(p.mes);
					jta_show_info.append(p.mes+"\r\n\r\n");
					
				}
			}
		}
		
		
	}

	
}

