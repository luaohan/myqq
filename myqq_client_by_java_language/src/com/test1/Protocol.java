package com.test1;

import java.io.*;
import java.net.*;

public class Protocol {

	static int packet_header_len = 4;
	static int packet_type_len = 2;
	static int packet_h_t_len = packet_header_len + packet_type_len;
	
    public static short UserName = 1;
    public static short PassWord = 2;
    public static short FriendList = 3;
    public static short ChatMess = 4;
    public static short LoginOk = 0;
    public static short LoginErr = 5;
    public static short FriendNums = 6;
    public static short ChatToFriend = 7;
    public static short FriendsStates = 8;
    public static short ON = 1;
    public static short OFF = 0;
    
	
    String mes = new String();
    short mes_type = -1;
    String ip = new String();
    int port = -1;
    
    Socket s;
    DataOutputStream out ;
	DataInputStream in;
	
	public Protocol(String mes, short mes_type, String ip, int port) {
		
		this.mes = mes;
		this.mes_type = mes_type;
		this.ip = ip;
		this.port = port;
		
		try {
			s = new Socket(ip, port);
			s.setKeepAlive(true);
			s.setTcpNoDelay(false);
			out = new DataOutputStream(s.getOutputStream());
			in = new DataInputStream(s.getInputStream());
			
		} catch (UnknownHostException e) { e.printStackTrace(); } catch (IOException e) {
			e.printStackTrace();
		}
		
	}
	
	public void send_packet(){
		try {
			
			int valid_packet_len = 
					Protocol.packet_header_len + Protocol.packet_type_len + mes.getBytes().length;
			//需要转换为网络字节序
			int packet_len = valid_packet_len;
			short packet_type = mes_type;
			
			out.writeInt(packet_len);       //包头
			out.writeShort(packet_type);    //包类型
			out.write(mes.getBytes());      //包体  
		
			out.flush();
		
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void recv_packet(){
		try {
			
			int packet_len = in.readInt();
			short packet_type = in.readShort();
			
			mes_type = packet_type;
			int packet_mes_len = packet_len - packet_h_t_len;
			
			if (packet_mes_len > 0){
				
				byte[] b = new byte[packet_mes_len]; 
				int read_size = in.read(b);
				mes = new String(b);
			}
			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

