#define CHAR 257
#define INTEGER 258
#define BOOLEAN 259
#define PERCENT 260
#define SPERCENT 261
#define MINUS_INTEGER 262
#define PLUS_INTEGER 263
#define MAZE_GRID_ID 264
#define SOLID_FILL_ID 265
#define MINES_ID 266
#define ROGUELEV_ID 267
#define MESSAGE_ID 268
#define MAZE_ID 269
#define LEVEL_ID 270
#define LEV_INIT_ID 271
#define GEOMETRY_ID 272
#define NOMAP_ID 273
#define OBJECT_ID 274
#define COBJECT_ID 275
#define MONSTER_ID 276
#define TRAP_ID 277
#define DOOR_ID 278
#define DRAWBRIDGE_ID 279
#define object_ID 280
#define monster_ID 281
#define terrain_ID 282
#define MAZEWALK_ID 283
#define WALLIFY_ID 284
#define REGION_ID 285
#define FILLING 286
#define IRREGULAR 287
#define JOINED 288
#define ALTAR_ID 289
#define LADDER_ID 290
#define STAIR_ID 291
#define NON_DIGGABLE_ID 292
#define NON_PASSWALL_ID 293
#define ROOM_ID 294
#define PORTAL_ID 295
#define TELEPRT_ID 296
#define BRANCH_ID 297
#define LEV 298
#define MINERALIZE_ID 299
#define CORRIDOR_ID 300
#define GOLD_ID 301
#define ENGRAVING_ID 302
#define FOUNTAIN_ID 303
#define POOL_ID 304
#define SINK_ID 305
#define TOILET_ID 306
#define NONE 307
#define RAND_CORRIDOR_ID 308
#define DOOR_STATE 309
#define LIGHT_STATE 310
#define CURSE_TYPE 311
#define ENGRAVING_TYPE 312
#define DIRECTION 313
#define RANDOM_TYPE 314
#define RANDOM_TYPE_BRACKET 315
#define A_REGISTER 316
#define ALIGNMENT 317
#define LEFT_OR_RIGHT 318
#define CENTER 319
#define TOP_OR_BOT 320
#define ALTAR_TYPE 321
#define UP_OR_DOWN 322
#define SUBROOM_ID 323
#define NAME_ID 324
#define FLAGS_ID 325
#define FLAG_TYPE 326
#define MON_ATTITUDE 327
#define MON_ALERTNESS 328
#define MON_APPEARANCE 329
#define ROOMDOOR_ID 330
#define IF_ID 331
#define ELSE_ID 332
#define TERRAIN_ID 333
#define HORIZ_OR_VERT 334
#define REPLACE_TERRAIN_ID 335
#define EXIT_ID 336
#define SHUFFLE_ID 337
#define QUANTITY_ID 338
#define BURIED_ID 339
#define LOOP_ID 340
#define FOR_ID 341
#define TO_ID 342
#define SWITCH_ID 343
#define CASE_ID 344
#define BREAK_ID 345
#define DEFAULT_ID 346
#define ERODED_ID 347
#define TRAPPED_STATE 348
#define RECHARGED_ID 349
#define INVIS_ID 350
#define GREASED_ID 351
#define FEMALE_ID 352
#define CANCELLED_ID 353
#define REVIVED_ID 354
#define AVENGE_ID 355
#define FLEEING_ID 356
#define BLINDED_ID 357
#define PARALYZED_ID 358
#define STUNNED_ID 359
#define CONFUSED_ID 360
#define SEENTRAPS_ID 361
#define ALL_ID 362
#define MONTYPE_ID 363
#define GRAVE_ID 364
#define ERODEPROOF_ID 365
#define FUNCTION_ID 366
#define MSG_OUTPUT_TYPE 367
#define COMPARE_TYPE 368
#define UNKNOWN_TYPE 369
#define rect_ID 370
#define fillrect_ID 371
#define line_ID 372
#define randline_ID 373
#define grow_ID 374
#define selection_ID 375
#define flood_ID 376
#define rndcoord_ID 377
#define circle_ID 378
#define ellipse_ID 379
#define filter_ID 380
#define complement_ID 381
#define gradient_ID 382
#define GRADIENT_TYPE 383
#define LIMITED 384
#define HUMIDITY_TYPE 385
#define STRING 386
#define MAP_ID 387
#define NQSTRING 388
#define VARSTRING 389
#define CFUNC 390
#define CFUNC_INT 391
#define CFUNC_STR 392
#define CFUNC_COORD 393
#define CFUNC_REGION 394
#define VARSTRING_INT 395
#define VARSTRING_INT_ARRAY 396
#define VARSTRING_STRING 397
#define VARSTRING_STRING_ARRAY 398
#define VARSTRING_VAR 399
#define VARSTRING_VAR_ARRAY 400
#define VARSTRING_COORD 401
#define VARSTRING_COORD_ARRAY 402
#define VARSTRING_REGION 403
#define VARSTRING_REGION_ARRAY 404
#define VARSTRING_MAPCHAR 405
#define VARSTRING_MAPCHAR_ARRAY 406
#define VARSTRING_MONST 407
#define VARSTRING_MONST_ARRAY 408
#define VARSTRING_OBJ 409
#define VARSTRING_OBJ_ARRAY 410
#define VARSTRING_SEL 411
#define VARSTRING_SEL_ARRAY 412
#define METHOD_INT 413
#define METHOD_INT_ARRAY 414
#define METHOD_STRING 415
#define METHOD_STRING_ARRAY 416
#define METHOD_VAR 417
#define METHOD_VAR_ARRAY 418
#define METHOD_COORD 419
#define METHOD_COORD_ARRAY 420
#define METHOD_REGION 421
#define METHOD_REGION_ARRAY 422
#define METHOD_MAPCHAR 423
#define METHOD_MAPCHAR_ARRAY 424
#define METHOD_MONST 425
#define METHOD_MONST_ARRAY 426
#define METHOD_OBJ 427
#define METHOD_OBJ_ARRAY 428
#define METHOD_SEL 429
#define METHOD_SEL_ARRAY 430
#define DICE 431

typedef union
{
    long    i;
    char    *map;
    struct {
        long room;
        long wall;
        long door;
    } corpos;
    struct {
        long area;
        long x1;
        long y1;
        long x2;
        long y2;
    } lregn;
    struct {
        long x;
        long y;
    } crd;
    struct {
        long ter;
        long lit;
    } terr;
    struct {
        long height;
        long width;
    } sze;
    struct {
        long die;
        long num;
    } dice;
    struct {
        long cfunc;
        char *varstr;
    } meth;
} YYSTYPE;
extern YYSTYPE yylval;
