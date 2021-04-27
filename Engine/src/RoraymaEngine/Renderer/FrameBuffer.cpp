#include "FrameBuffer.hh"

#include <glad/glad.h>
#include "../Core/Assert.hh"
#include <iostream>

rym::api::FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
	:
	m_Size({ width, height })
{
	CreateFrameBuffer();
}

rym::api::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_FrameBufferID);
	glDeleteTextures(1, &m_ColorAttachment);
	glDeleteTextures(1, &m_DepthStencilAttachment);
	glDeleteTextures(1, &m_EntitysPickAttachment);
}

void rym::api::FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	glViewport(0, 0, m_Size.width, m_Size.height);
}

void rym::api::FrameBuffer::UnBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t rym::api::FrameBuffer::GetColorAttachment()
{
	return m_ColorAttachment;
}

void rym::api::FrameBuffer::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0)
	{
		RYM_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
		return;
	}

	m_Size = { width, height };

	CreateFrameBuffer();
}

int rym::api::FrameBuffer::ReadEntitys(int x, int y)
{
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	int pixelData;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

	return pixelData;
}

void rym::api::FrameBuffer::ClearEntitysAttachment()
{
	int junkValue = -1;
	glClearTexImage(m_EntitysPickAttachment, 0, GL_RED_INTEGER, GL_INT, &junkValue);
}

void rym::api::FrameBuffer::CreateFrameBuffer()
{
	if (m_FrameBufferID)
	{
		glDeleteFramebuffers(1, &m_FrameBufferID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_EntitysPickAttachment);
		glDeleteTextures(1, &m_DepthStencilAttachment);
	}

	// Bind, create framebuffer
	glCreateFramebuffers(1, &m_FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

	// Color attachment
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Size.width, m_Size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthStencilAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthStencilAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Size.width, m_Size.height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilAttachment, 0);

	// Entitys pick attachment
	glCreateTextures(GL_TEXTURE_2D, 1, &m_EntitysPickAttachment);
	glBindTexture(GL_TEXTURE_2D, m_EntitysPickAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Size.width, m_Size.height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_EntitysPickAttachment, 0);



	// Hay que especificar a opengl que queremos multiples render targets
	uint32_t buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, buffers);
	

	RYM_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
