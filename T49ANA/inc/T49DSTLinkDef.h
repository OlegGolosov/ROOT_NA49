#ifdef __CINT__

/*
$Log: T49DSTLinkDef.h,v $
Revision 1.11  2002/02/04 16:03:28  cblume
Add T49Veto

Revision 1.10  2001/12/12 17:34:07  cblume
Remove obsolete classes

Revision 1.9  2001/11/12 10:46:27  cblume
Update for ROOT v3.01

 * Revision 1.8  2000/11/09  09:06:47  cblume
 * Introduce byte count option
 *
 * Revision 1.7  2000/05/19  15:47:39  cblume
 * Added T49Dst classes again
 *
 * Revision 1.6  2000/04/13  14:53:42  cblume
 * Remove files
 *
 * Revision 1.5  2000/02/23  15:45:32  cblume
 * T49ParticleMCRoot now with customized streamer
 *
 * Revision 1.4  2000/02/23  10:05:03  cblume
 * T49VertexMCRoot has now a customized streamer
 *
 * Revision 1.3  1999/11/25  13:57:51  cblume
 * Introduced gT49KeepEvent in T49Event(MC)Root to make events cloneable
 *
 * Revision 1.2  1999/11/23  12:51:07  cblume
 * Remove files
 *
*/

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ global gT49;
#pragma link C++ global gT49KeepEvent; 

#pragma link C++ class T49Run-;
#pragma link C++ class T49Event-;  
#pragma link C++ class T49EventRoot-;  
#pragma link C++ class T49EventMCRoot-;  
#pragma link C++ class T49Particle-;  
#pragma link C++ class T49ParticleRoot-;
#pragma link C++ class T49ParticleIdRoot-;
#pragma link C++ class T49ParticleMCRoot-;
#pragma link C++ class T49MonitorTPC-;
#pragma link C++ class T49MonitorTPCRoot-;
#pragma link C++ class T49Beam-;
#pragma link C++ class T49BeamRoot-;
#pragma link C++ class T49Vertex-;
#pragma link C++ class T49VertexRoot-;
#pragma link C++ class T49VertexMCRoot-;
#pragma link C++ class T49Ring-;
#pragma link C++ class T49RingRoot-;
#pragma link C++ class T49Veto+;
#pragma link C++ class T49VetoRoot+;

#endif   
