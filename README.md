# OpenGL Sandbox

Environment for creation, education, and experimentation with OpenGL.

## Projects

<table>
	<tr>
		<th>Title</th>
		<th>Description</th>
	</tr>
	<tr>
		<th colspan=2>Libraries</th>
	</tr>
	<tr>
		<td><a href="./include/opengl-sandbox/logger.h">Logger</a></td>
		<td>Logger class used to log to console and/or file</td>
	</tr>
	<tr>
		<td><a href="./include/opengl-sandbox/arcball.h">Arcball</a></td>
		<td>Arcball class that utilizes quaternions to rotate objects</td>
	</tr>
	<tr>
		<td><a href="./include/opengl-sandbox/camera_arcball.h">Camera Arcball</a></td>
		<td>Orbital camera derived from an arcball</td>
	</tr>
	<tr>
		<th colspan=2>Programs</th>
	</tr>
	<tr>
		<td><a href="./src/triangle">Triangle</a></td>
		<td>Rendering a triangle</td>
	</tr>
	<tr>
		<td><a href="./src/cube_ebo">Cube EBO</a></td>
		<td>Rendering a cube via an element buffer object</td>
	</tr>
	<tr>
		<td><a href="./src/cube_shaded">Cube Shaded</a></td>
		<td>Rendering a cube shaded via the Blinn-Phong shading model</td>
	</tr>
	<tr>
		<td><a href="./src/cube_textured">Cube Textured</a></td>
		<td>Rendering a texture onto the surface of a cube shaded via the Blinn-Phong shading model</td>
	</tr>
	<tr>
		<td><a href="./src/cube_arcball">Cube Arcball</a></td>
		<td>Viewing a shaded, textured cube rotated via an arcball</td>
	</tr>
	<tr>
		<td><a href="./src/cube_camera_arcball">Cube Camera Arcball</a></td>
		<td>Viewing a shaded, textured cube via an orbital camera</td>
	</tr>
</table>

## Setup

See [SETUP.md](./SETUP.md) to learn how this repository was configured.
