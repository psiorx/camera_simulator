using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Text;
using System.Net;
using System.Net.Sockets;
	
public class CameraScript : MonoBehaviour {
	public RenderTexture cameraImage;
	public string host = "localhost";
	public int port = 12345; 
	private Texture2D myTexture2D;
	private TcpClient client;
	private Stream stream;

	void Connect() {
		try {
			Debug.Log ("Attempting Connection");
			client = new TcpClient ();
			client.Connect (host, port);
			stream = client.GetStream (); 
		} catch(Exception ex) {
			Debug.Log (ex.Message);
		}
	}

	void Start () {
		myTexture2D = new Texture2D (cameraImage.width, cameraImage.height, TextureFormat.RGB24, false);
		Connect ();
	}

	void Update () {
		RenderTexture.active = cameraImage;		                                      
		myTexture2D.ReadPixels (new Rect (0, 0, cameraImage.width, cameraImage.height), 0, 0);
		//byte[] imageBytes = myTexture2D.EncodeToPNG ();
		byte[] imageBytes = myTexture2D.GetRawTextureData ();
		if (client.Connected) {
			Debug.Log (string.Format ("sending {0} bytes of image data", imageBytes.Length));
			byte [] widthBytes = BitConverter.GetBytes (cameraImage.width);
			byte [] heightBytes = BitConverter.GetBytes (cameraImage.height);
			stream.Write(widthBytes, 0, 4);
			stream.Write(heightBytes, 0, 4);
			stream.Write(imageBytes, 0, imageBytes.Length);
		} else {
			Connect ();
		}
	}
}
