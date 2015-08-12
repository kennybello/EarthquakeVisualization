#include "Earth.h"
#include "config.h"

using namespace std;
const double RADIUS = 1.0;
Earth::Earth(shared_ptr<Camera> camera, shared_ptr<Light> light) {
	defaultCamera = camera;
	this->light = light;

	// Note: TEXTURE_PATH is set in config.h
	tex = Texture::fromFile(TEXTURE_PATH);
	setupGeometry();
	setupShader();
}

void Earth::setupGeometry() {
  // TODO: Write your own code here to tesselate and create texture coordinates for the Earth 
  // and then save the data to VertexBuffers, AttributeArrays, and an IndexStream.  The tesselation
  // should be based on the STACKS and SLICES parameters.
	const int STACKS = 20;
	const int SLICES = 40;
	
	Array<Vector3> cpuVerts, cpuNorms;
	Array<int> cpuIndices;
	Array<Vector2> texVerts;

	double delta_theta, delta_phi, theta, phi;
	delta_theta = pi() / STACKS;
	delta_phi = 2*pi() / SLICES;
	theta = 0; phi = 0;		
	
	Vector3 first = Vector3(0, 1, 0);// North Pole
	Vector3 corner,corner2;
	int x = 0, y = 1, p = 0;
	for (int vertical = 0; vertical < STACKS; vertical++) {
		phi = 0;
		for (int horizontal = 0; horizontal <= SLICES; horizontal++) {
			corner = getPosition(theta + delta_theta, phi);
			phi += delta_phi;
			//corner2 = getPosition(theta+delta_theta, phi);
			Vector3 last = getPosition(theta, phi);
			Vector3 cross = (first - corner).cross(last - corner);
			cpuVerts.append(first, corner);
			cpuNorms.append(cross, cross);
			if (horizontal >0){
				cpuIndices.append(x-2, y-2, y, y, x, x-2);
			}
			texVerts.append(Vector2(((horizontal*1.0))/SLICES, (vertical * 1.0)/STACKS),
				Vector2((horizontal* 1.0)/SLICES, ((vertical*1.0)+1)/STACKS));
			y += 2;
			x += 2;
			debugPrintf("%d, corner: %f, %f, %f \n last: %f, %f, %f \n %f, %f \n", 
				p++, corner.x, corner.y, corner.z, last.x, last.y, last.z, phi, theta);
			if (horizontal == SLICES){
				first = corner;
			}
			else{
				first = last;
			}
		}
		theta += delta_theta;
	}

	vbuffer = VertexBuffer::create((sizeof(Vector3) + sizeof(Vector3) + sizeof(Vector2)) * cpuVerts.size() +
		sizeof(int)*cpuIndices.size());
	debugAssertGLOk();

	gpuVerts = AttributeArray(cpuVerts, vbuffer);
	debugAssertGLOk();
	gpuNorms = AttributeArray(cpuNorms, vbuffer);
	debugAssertGLOk();
	gpuIndices = IndexStream(cpuIndices, vbuffer);
	debugAssertGLOk();
	gpuTexCoords = AttributeArray(texVerts, vbuffer);
	debugAssertGLOk();
	

}

Vector3 Earth::getPosition(double latitude, double longitude) {
	return Vector3(
		RADIUS * sin(longitude) * sin(latitude), 
		RADIUS * cos(latitude), 
		RADIUS * cos(longitude) * sin(latitude)
	);

}

void Earth::setupShader() {
	G3D::String vertexShaderPath = DATA_DIR+"gouraud-shader.vrt";
	G3D::String fragmentShaderPath = DATA_DIR+"gouraud-shader.pix";
	//G3D::String vertexShaderPath = "texture-shader.vrt";
    //G3D::String fragmentShaderPath = "texture-shader.pix";
	debugAssert(FileSystem::exists(vertexShaderPath)); // If this asserts false, then your DATA_DIR is not set correctly in config.h
	debugAssert(FileSystem::exists(fragmentShaderPath));
	shader = Shader::fromFiles(vertexShaderPath, fragmentShaderPath);
}

void Earth::configureShaderArgs(RenderDevice* rd) {

	rd->clear();

	rd->pushState();

//	rd->setRenderMode(RenderDevice::RENDER_WIREFRAME); 
	Args args;
	args.enableG3DArgs(true);
	args.setUniform("wsLight", light->position().xyz().direction());
	args.setUniform("lightColor", light->color);
	args.setUniform("wsEyePosition", defaultCamera->frame().translation);

	args.setAttributeArray("texCoord0", gpuTexCoords);
	args.setUniform("textureSampler", tex, Sampler::video());

	args.setAttributeArray("vertex", gpuVerts);
	args.setAttributeArray("normal", gpuNorms);

	args.setIndexStream(gpuIndices);
	args.setPrimitiveType(PrimitiveType::TRIANGLES);
	
	//TODO: Set the attribute arrays and primitive types to draw the geometry
	//      that you created in setupGeometry().
	
	rd->apply(shader, args);	
	
	rd->popState();

}

void Earth::draw(RenderDevice *rd) {
  rd->pushState();
  rd->setCullFace(CullFace::NONE);

  configureShaderArgs(rd);
  //Draw::axes(CoordinateFrame(), rd);
  rd->setRenderMode(RenderDevice::RENDER_WIREFRAME);
  rd->popState();
}

