import PyScripting
from PyScripting import Math, Input

#VERY IMPORTANT DO NOT CHANGE THE FILE NAME.

class Sprite(PyScripting.PyScript):
	def OnStart(self):
		pass
	def OnUpdate(self, _delta):
		velocidad = 40.0
		transform = self.entity.GetTransformComponent()
		if(Input.IsKeyPressed(Input.KeyCode.D)):
			transform.position.x += velocidad * _delta
		elif(Input.IsKeyPressed(Input.KeyCode.A)):
			transform.position.x -= velocidad * _delta
		if(Input.IsKeyPressed(Input.KeyCode.W)):
			transform.position.y += velocidad * _delta
		elif(Input.IsKeyPressed(Input.KeyCode.S)):
			transform.position.y -= velocidad * _delta
		pass
	def OnQuit(self):
		pass

