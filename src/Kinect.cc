#include "Kinect.h"

void Depth::roberts_cross(){
	for( int i = 0; i < DIM_Y; i++ ){
		for( int j = 0; j < DIM_X; j++ ){
			if( abs( depth[i*DIM_X+j] - depth[(i+1)*DIM_X+(j+1)] )+
				abs( depth[(i+1)*DIM_X+j] - depth[i*DIM_X+(j+1)] ) )
				depth[i*DIM_X+j] = 1;
			else
				depth[i*DIM_X+j] = 0;
		}
	}

}

void Depth::dilate(){
	for( int i = 0; i < DIM_Y; i++ ){
		for( int j = 0; j < DIM_X; j++ ){
			if( depth[i*DIM_X+j] == 1 ){
				if( i > 0 && depth[(i-1)*DIM_X+j] == 0 ) depth[(i-1)*DIM_X+j] = 2;
				if( j > 0 && depth[(i)*DIM_X+j-1] == 0 ) depth[(i)*DIM_X+j-1] = 2;
				if( i+1 < DIM_Y && depth[(i+1)*DIM_X+j] == 0 ) depth[(i+1)*DIM_X+j] = 2;
				if( j+1 < DIM_X && depth[(i)*DIM_X+j+1] == 0 ) depth[(i)*DIM_X+j+1] = 2;
			}
		}
	}
	
	for( int i = 0; i < DIM_Y; i++ ){
		for( int j = 0; j < DIM_X; j++ ){
			if( depth[i*DIM_X+j] == 2 )
				depth[i*DIM_X+j] = 1;
		}
	}
}

void Depth::get_depth(){
	uint32_t ts;
	depth = 0;
	if( freenect_sync_get_depth( (void**)&depth, &ts, 0, FREENECT_DEPTH_11BIT) < 0 ){
		cerr << "NO KINECT\n";
		exit( -1 );
	}
	for( int i = 0; i < DIM_Y; i++ ){
		for( int j = 0; j < DIM_X; j++ ){
			if( depth[i*DIM_X+j] < range )
				depth[i*DIM_X+j] = 1;
			else
				depth[i*DIM_X+j] = 0;
		}
	}
}

void Depth::standard_collection(bool test = false){
	get_depth();
	if(test) save_depth("PreConvolutions");

	dilate();
	dilate();
	dilate();
	dilate();
	roberts_cross();

	if(test) save_depth("PostConvolutions");
}

vector<pair<int,int>> Depth::get_coords(){
	vector<pair<int,int>> v;
	pair<int, int> p = {0,0};

	standard_collection();

	for( int i = 0; i < DIM_Y; i++){
		for( int j = 0; j < DIM_X; j++ ){
			if( depth[i*DIM_X+j] == 1 ){
				p.first = DIM_X - j;	// Mirrored for niceness
				p.second = i;
				v.push_back(p);
			}
		}
	}
	
	return v;
}

void Depth::save_depth( const string& filename ){
	ofstream ofile;
	ofile.open( filename+".pbm" );
	
	ofile << 'P';
	ofile << '1';
	ofile << '\n';
	ofile << "640 480\n";
	
	for( int i = 0; i < DIM_Y; i++ ){
		for( int j = 0; j < DIM_X; j++ ){
			if( depth[i*DIM_X+j] )
				ofile << 1;
			else
				ofile << 0;
		}
		ofile << '\n';
	}

	ofile.close();
}

void Depth::snapshot( ){
	freenect_sync_set_led( LED_RED, 0 );
	sleep( 4 );
	get_depth();
	freenect_sync_set_led( LED_GREEN, 0 );
}
