uniform sampler2D texture;

void main()
{
    // Get the color of the current pixel from the texture
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Extract the red component (x) from the input pixel
    float x = color.r;

    // Perform the computation for each color component
    float r = 9.0 * (1.0 - x) * x * x * x;
    float g = 15.0 * (1.0 - x) * (1.0 - x) * x * x;
    float b = 8.5 * (1.0 - x) * (1.0 - x) * (1.0 - x) * x;

    // Set the output color
    gl_FragColor = vec4(r, g, b, 1.0);
}
