#version 330 core

in vec4 vertex_color;
out vec4 FragColor;

uniform bool aplicaVimet;

void main() {
    if(aplicaVimet){
    	
    	//He afegit 2 variables per fer més clar el codi.
    	int coordX = int(gl_FragCoord.x);
    	int coordY = int(gl_FragCoord.y);
    
        if(coordX % 20 > 10 && coordY % 20 > 10) discard;
        
        
        //El codi està fet amb dos ifs per aclariment.
        if (coordX % 40 < 10 && (coordY % 40 < 20 || coordY % 40 > 30)) {        
            FragColor = vec4(vertex_color.rgb * 0.5, 1.0f);
        }
        else if (coordX % 40 >= 20 && coordX % 40 < 30 && coordY % 40 > 10) {        
            FragColor = vec4(vertex_color.rgb * 0.5, 1.0f);
        }
        else FragColor = vertex_color;
    }
    else FragColor = vertex_color; 
}
