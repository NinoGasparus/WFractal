uniform sampler2D texture;

void main()
{
    // Get the color of the current pixel from the texture
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Extract the red component (t) from the input pixel
    float t = color.r;

    // Perform the computation for each color component
    float r = 2.0 * t;
    float g = (r / 2.0) * 0.22;
    float b = (r / 2.0) * 0.33;

    // Set the output color
    gl_FragColor = vec4(r, g, b, 1.0);
}
