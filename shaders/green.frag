uniform sampler2D texture;

void main()
{
    // Get the color of the current pixel from the texture
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Extract the red component (x) from the input pixel
    float x = color.r;

    // Perform the computation for each color component
    float r = 1.0 / (-x) + 2.0;
    float g = 2.0 * x;
    float b = (r + g) * 0.5;

    // Set the output color
    gl_FragColor = vec4(r, g, b, 1.0);
}
