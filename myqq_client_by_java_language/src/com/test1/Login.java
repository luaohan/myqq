package com.test1;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.SocketException;
import javax.swing.*;

public class Login extends JFrame implements ActionListener{

	JPanel jp_qq_num, jp_qq_passwd, jp_login_cancel;
	JLabel jl_qq_num, jl_qq_passwd;
	JButton jb_login, jb_cancel;
	JTextField jtf_qq_num;
	JPasswordField jpf_qq_passwd;
	
	public Login() {
		
		jp_qq_num = new JPanel();
		jp_qq_passwd = new JPanel();
		jp_login_cancel = new JPanel();
		
		jl_qq_num = new JLabel("QQ 号");
		jl_qq_passwd = new JLabel("密   码");
		
		jb_login = new JButton("登录");
		jb_login.addActionListener(this); //加监听
		jb_cancel = new JButton("取消");   //暂时没用
		
		jtf_qq_num = new JTextField(10);
		jpf_qq_passwd = new JPasswordField(10);

		this.setLayout(new GridLayout(3,1));
		
		jp_qq_num.add(jl_qq_num);
		jp_qq_num.add(jtf_qq_num);
		
		jp_qq_passwd.add(jl_qq_passwd);
		jp_qq_passwd.add(jpf_qq_passwd);
		
		jp_login_cancel.add(jb_login);
		jp_login_cancel.add(jb_cancel);
		
		this.add(jp_qq_num);
		this.add(jp_qq_passwd);
		this.add(jp_login_cancel);
		
		this.setTitle("登录");
		this.setResizable(false);   //进制改变窗口的大小
//		this.setUndecorated(true);  //不显示边框
		this.setIconImage((new ImageIcon("image/qq.png")).getImage());
		this.setSize(350,200);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		//设置窗口居中
		int width=getToolkit().getDefaultToolkit().getScreenSize().width;
		int height=getToolkit().getDefaultToolkit().getScreenSize().height;
		this.setLocation(width/2-200,height/2-200);
				
		this.setVisible(true);
		
	}

	public static void main(String[] args) {
		Login lg = new Login();
	}
	
	public void actionPerformed(ActionEvent e) {
		
		String username = jtf_qq_num.getText().trim();
		String passwd = new String(jpf_qq_passwd.getPassword()).trim();
		
		if(e.getSource() == jb_login){//按下登录键
			
			if (username.equals("")){ //用户名不能为空
				JOptionPane.showMessageDialog(this, "请输入用户名");
				return ;
			}
			
			if (passwd.equals("")){ //密码不能为空
				JOptionPane.showMessageDialog(this, "请输入密码");
				return ;
			}
			
			//创建用户名包体
			Protocol p 
				= new Protocol(username, Protocol.UserName, "192.168.220.171", 8899);
			//向服务器发送用户名
			p.send_packet();
			//接收服务器的回应信息
			p.recv_packet();
			
			if(p.mes_type == Protocol.LoginOk){//用户名正确
				//打包密码信息
				p.mes = passwd;
				p.mes_type = Protocol.PassWord;
				//发送到服务器
				p.send_packet();
				//接收服务器的回应信息
				p.recv_packet();
				
				if(p.mes_type == Protocol.LoginOk){//密码正确
					//向服务器申请好友列表
					p.mes = "u";//没用
					p.mes_type = Protocol.FriendList;
					
					p.send_packet(); //发送
					p.recv_packet(); //接收
					
					String friendnames[];
					if(p.mes_type == Protocol.FriendList){
						 friendnames = p.mes.split(" ");
						 //启动聊天界面
						 Chat c = new Chat(friendnames, p, username);
					}
					
					this.dispose(); //关闭当前窗口
					
				}else if(p.mes_type == Protocol.LoginErr){
					JOptionPane.showMessageDialog(this, "密码错误");
				}
			}else if(p.mes_type == Protocol.LoginErr){
				JOptionPane.showMessageDialog(this, "用户名错误");
			}
		}
		
	}


}
