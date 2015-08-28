using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Text;
using System.Net;
using System.Net.Sockets;

public class PoseGrabber : MonoBehaviour {
	public string host = "localhost";
	public int port = 12346; 

	private TcpClient client;
	private Stream stream;
	
	void Connect() {
		try {
			Debug.Log ("Pose Grabber Attempting Connection");
			client = new TcpClient ();
			client.Connect (host, port);
			stream = client.GetStream (); 
		} catch(Exception ex) {
			Debug.Log (ex.Message);
		}
	}
	
	void Start () {
		Connect ();
	}

	float[] GrabPose() {
		const int POSE_LENGTH = 6;
		byte[] poseBytes = new byte[POSE_LENGTH * sizeof(float)];
		int bytesRead = stream.Read (poseBytes, 0, poseBytes.Length);
		float[] pose = new float[POSE_LENGTH];
		if (bytesRead == poseBytes.Length) {
			for (int x = 0; x < POSE_LENGTH; x++) {
				pose [x] = BitConverter.ToSingle (poseBytes, 4 * x);
				Debug.Log (string.Format ("pose[{0}] = {1}", x, pose [x]));
			}
		} else {
			Debug.Log ("Failed to read a pose from the server");
			return null;
		}
		return pose;
	}

	void SetCameraPose(float[] pose) {
		transform.position = new Vector3 (pose [0], pose [1], pose [2]);
		transform.eulerAngles = new Vector3 (pose [3], pose [4], pose [5]);
	}

	void Update () {
		if (client.Connected) {
			float[] pose = GrabPose();
			if(pose != null) {
				SetCameraPose(pose);
			}
		} else {
			Connect ();
		}	
	}
}
