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
		
		jl_qq_num = new JLabel("QQ ��");
		jl_qq_passwd = new JLabel("��   ��");
		
		jb_login = new JButton("��¼");
		jb_login.addActionListener(this); //�Ӽ���
		jb_cancel = new JButton("ȡ��");   //��ʱû��
		
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
		
		this.setTitle("��¼");
		this.setResizable(false);   //���Ƹı䴰�ڵĴ�С
//		this.setUndecorated(true);  //����ʾ�߿�
		this.setIconImage((new ImageIcon("image/qq.png")).getImage());
		this.setSize(350,200);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		//���ô��ھ���
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
		
		if(e.getSource() == jb_login){//���µ�¼��
			
			if (username.equals("")){ //�û�������Ϊ��
				JOptionPane.showMessageDialog(this, "�������û���");
				return ;
			}
			
			if (passwd.equals("")){ //���벻��Ϊ��
				JOptionPane.showMessageDialog(this, "����������");
				return ;
			}
			
			//�����û�������
			Protocol p 
				= new Protocol(username, Protocol.UserName, "192.168.220.171", 8899);
			//������������û���
			p.send_packet();
			//���շ������Ļ�Ӧ��Ϣ
			p.recv_packet();
			
			if(p.mes_type == Protocol.LoginOk){//�û�����ȷ
				//���������Ϣ
				p.mes = passwd;
				p.mes_type = Protocol.PassWord;
				//���͵�������
				p.send_packet();
				//���շ������Ļ�Ӧ��Ϣ
				p.recv_packet();
				
				if(p.mes_type == Protocol.LoginOk){//������ȷ
					//���������������б�
					p.mes = "u";//û��
					p.mes_type = Protocol.FriendList;
					
					p.send_packet(); //����
					p.recv_packet(); //����
					
					String friendnames[];
					if(p.mes_type == Protocol.FriendList){
						 friendnames = p.mes.split(" ");
						 //�����������
						 Chat c = new Chat(friendnames, p, username);
					}
					
					this.dispose(); //�رյ�ǰ����
					
				}else if(p.mes_type == Protocol.LoginErr){
					JOptionPane.showMessageDialog(this, "�������");
				}
			}else if(p.mes_type == Protocol.LoginErr){
				JOptionPane.showMessageDialog(this, "�û�������");
			}
		}
		
	}


}
