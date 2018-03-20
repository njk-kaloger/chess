	my_string p = read_string("Enter Piece Name: Eg Pawn, Knight ");
	if (strcmp(p.str, "Pawn") == 0) {
	 	move->piece->type = Pawn;
	} else if (strcmp(p.str, "Knight") == 0) {
	 	move->piece->type = Knight;
	} else if (strcmp(p.str, "King") == 0) {
	 	move->piece->type = King;
	} else if (strcmp(p.str, "Queen") == 0) {
	 	move->piece->type = Queen;
	} else if (strcmp(p.str, "Bishop") == 0) {
	 	move->piece->type = Bishop;
	} else if (strcmp(p.str, "Castle") == 0) {
	 	move->piece->type = Castle;
	} else {
		printf("Piece doesn't Exist\n");
		return NULL;
	}


	if (get_piece(b, (m->row_end), (m->col_end)))
				{	
					int pos = 0;
					int size = sizeof(b->pieces) / sizeof(b->pieces[0]);
					for(int count = 0 ; count < 32 ; count++) {
						if(b->pieces[count]->row == (m->row_end) && b->pieces[count]->col == (m->col_end)) {
							pos = count;
						}
					}
					for(int i = pos-1; i < size-1; i++)
					{
						b->pieces[i] = b->pieces[i + 1];
					}
					size--;
				}