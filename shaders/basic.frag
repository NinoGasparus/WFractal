uniform sampler2D texture;

void main()
{       //Default shader- Does nothing
       gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}
