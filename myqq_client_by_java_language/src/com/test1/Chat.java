package com.test1;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;

public class Chat extends JFrame implements ActionListener,MouseListener{

	JSplitPane jspp = null;  //JSplitPane 用于分隔两个（只能两个）Component(jp_right,jp_left)
	
	JPanel jp_right = null;  //右边的jpanel
	JPanel jp_left = null;   //左边的jpanel
	
	JLabel[] jlabel;         //承放好友，左边的好友列表
	
	JTextArea jta_show_info = null;    //显示信息
	JTextArea jta_send_info = null;    //用于输入信息
	
	JScrollPane jsp_jta_show_info = null;  //承放jta_show_info
	JScrollPane jsp_jta_send_info = null;  //承放jta_send_info
	
	
	JPanel jp_for_button = null;      //承放发送按钮
	
	JButton button_send = null;       //发送按钮
	
	Protocol p = null;   //协议对象
	
	String username = null;
	
	String chattofriendname = null;
	
	String[] friendnames;
	
	public Chat(String[] friendnames, Protocol p, String username){
		
		this.friendnames = friendnames;
		this.username = username;
		this.p = p;
		
		jta_show_info = new JTextArea(13, 5);
		jta_show_info.setEnabled(false);      //禁止编辑
		
		jta_send_info = new JTextArea(5, 5);
		
		jsp_jta_show_info = new JScrollPane(jta_show_info);
		jsp_jta_send_info = new JScrollPane(jta_send_info);
		
		jp_for_button = new JPanel();
		
		jp_right = new JPanel(new BorderLayout());
		jp_left = new JPanel(new GridLayout(friendnames.length, 1));

		jlabel = new JLabel[friendnames.length];
		
		for(int i = 0; i < jlabel.length; i++){   
			jlabel[i] = new JLabel(friendnames[i],(new ImageIcon("image/qq.png")),JLabel.LEFT);
			jlabel[i].addMouseListener(this);       //监听双击事件
			jp_left.add(jlabel[i]);
			
		}
		
		//开启一个线程在那里读信息
		//必须在初始化jlabel[] 数组之后，因为到时候会改变标签的颜色
		ReadInfo thread =new ReadInfo(p);
		thread.start();
				
		button_send = new JButton("点击发送");
		button_send.addActionListener(this);       //监听点击事件
		
		jp_for_button.add(button_send);
		
		jp_right.add(jsp_jta_show_info,BorderLayout.NORTH);
		jp_right.add(jsp_jta_send_info);
		jp_right.add(jp_for_button, BorderLayout.SOUTH);
		
		jspp = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,jp_left,jp_right);
		//使左右两个jpanel 可以折叠
		jspp.setOneTouchExpandable(true);  
		
		this.add(jspp);
		this.setTitle(username + "正在聊天");
		this.setIconImage((new ImageIcon("image/qq.png")).getImage());
		this.setResizable(false);  //不可以改变窗口的大小
		this.setSize(600, 400);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setLocation(300, 200);
		this.setVisible(true);
	}
	

	public void actionPerformed(ActionEvent e) {
		
		if (e.getSource() == button_send){  //点击发送
			if (chattofriendname != null){  //双击了好友
				
				//检查好友是否在线，黑色代表不在线，不可以聊天
				for (int i = 0; i < friendnames.length; i++){
					if (jlabel[i].getText().equals(chattofriendname)){
						if (jlabel[i].getForeground() == Color.black){
							JOptionPane.showMessageDialog(this, "sorry, your friend is not online");
							return ;
						}
					}
				}
				//到达这里说明在线，可以发送信息
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
			//得到好友名字
			chattofriendname = ((JLabel)e.getSource()).getText();
			this.setTitle(username + " 正在与  "+chattofriendname+" 聊天");
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

	
class ReadInfo extends Thread{  //这个类负责接收信息
		
		Protocol p = null;
		
		public ReadInfo(Protocol p){
			this.p = p;
		}
		
		public void run(){
			
			while(true){
				//阻塞接收信息
				p.recv_packet();
				
				if (p.mes_type == p.FriendsStates){
					
					String[] friendstates = p.mes.split(" ");
					//根据好友的状态设置好友姓名的颜色
					for (int i = 0; i < friendnames.length; i++){
						if (friendstates[i].equals("1")){
							jlabel[i].setForeground(Color.green);
							jlabel[i].setEnabled(true);
						}else if (friendstates[i].equals("0")){
							jlabel[i].setForeground(Color.black);
							jlabel[i].setEnabled(false);
						}
					}
					
				}else if (p.mes_type == p.ON){   //好友上线
					//找到这个好友，并设置颜色
					for (int i = 0; i < friendnames.length; i++){
						if (jlabel[i].getText().equals(p.mes)){
							jlabel[i].setForeground(Color.green);
							jlabel[i].setEnabled(true);
							break;
						}
					}
					
				}else if(p.mes_type == p.OFF){   //好友下线
					//找到这个好友，并设置颜色
					for (int i = 0; i < friendnames.length; i++){
						if (jlabel[i].getText().equals(p.mes)){
							jlabel[i].setForeground(Color.black);
							jlabel[i].setEnabled(false);
							break;
						}
					}
					
				}else{ //发来的为聊天信息
					
//					jta_show_info.setText(p.mes);
					jta_show_info.append(p.mes+"\r\n\r\n");
					
				}
			}
		}
		
		
	}

	
}

