//
//  Shader.fsh
//  ArrowTest
//
//  Created by Daniel on 31/03/2013.
//  Copyright (c) 2013 Daniel Conde Linares. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
