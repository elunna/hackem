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
#define VENT_ID 304
#define POOL_ID 305
#define SINK_ID 306
#define TOILET_ID 307
#define NONE 308
#define RAND_CORRIDOR_ID 309
#define DOOR_STATE 310
#define LIGHT_STATE 311
#define CURSE_TYPE 312
#define ENGRAVING_TYPE 313
#define DIRECTION 314
#define RANDOM_TYPE 315
#define RANDOM_TYPE_BRACKET 316
#define A_REGISTER 317
#define ALIGNMENT 318
#define LEFT_OR_RIGHT 319
#define CENTER 320
#define TOP_OR_BOT 321
#define ALTAR_TYPE 322
#define UP_OR_DOWN 323
#define SUBROOM_ID 324
#define NAME_ID 325
#define FLAGS_ID 326
#define FLAG_TYPE 327
#define MON_ATTITUDE 328
#define MON_ALERTNESS 329
#define MON_APPEARANCE 330
#define ROOMDOOR_ID 331
#define IF_ID 332
#define ELSE_ID 333
#define TERRAIN_ID 334
#define HORIZ_OR_VERT 335
#define REPLACE_TERRAIN_ID 336
#define EXIT_ID 337
#define SHUFFLE_ID 338
#define QUANTITY_ID 339
#define BURIED_ID 340
#define LOOP_ID 341
#define FOR_ID 342
#define TO_ID 343
#define SWITCH_ID 344
#define CASE_ID 345
#define BREAK_ID 346
#define DEFAULT_ID 347
#define ERODED_ID 348
#define TRAPPED_STATE 349
#define RECHARGED_ID 350
#define INVIS_ID 351
#define GREASED_ID 352
#define FEMALE_ID 353
#define CANCELLED_ID 354
#define REVIVED_ID 355
#define AVENGE_ID 356
#define FLEEING_ID 357
#define BLINDED_ID 358
#define PARALYZED_ID 359
#define STUNNED_ID 360
#define CONFUSED_ID 361
#define SEENTRAPS_ID 362
#define ALL_ID 363
#define MONTYPE_ID 364
#define GRAVE_ID 365
#define ERODEPROOF_ID 366
#define FUNCTION_ID 367
#define MSG_OUTPUT_TYPE 368
#define COMPARE_TYPE 369
#define UNKNOWN_TYPE 370
#define rect_ID 371
#define fillrect_ID 372
#define line_ID 373
#define randline_ID 374
#define grow_ID 375
#define selection_ID 376
#define flood_ID 377
#define rndcoord_ID 378
#define circle_ID 379
#define ellipse_ID 380
#define filter_ID 381
#define complement_ID 382
#define gradient_ID 383
#define GRADIENT_TYPE 384
#define LIMITED 385
#define HUMIDITY_TYPE 386
#define STRING 387
#define MAP_ID 388
#define NQSTRING 389
#define VARSTRING 390
#define CFUNC 391
#define CFUNC_INT 392
#define CFUNC_STR 393
#define CFUNC_COORD 394
#define CFUNC_REGION 395
#define VARSTRING_INT 396
#define VARSTRING_INT_ARRAY 397
#define VARSTRING_STRING 398
#define VARSTRING_STRING_ARRAY 399
#define VARSTRING_VAR 400
#define VARSTRING_VAR_ARRAY 401
#define VARSTRING_COORD 402
#define VARSTRING_COORD_ARRAY 403
#define VARSTRING_REGION 404
#define VARSTRING_REGION_ARRAY 405
#define VARSTRING_MAPCHAR 406
#define VARSTRING_MAPCHAR_ARRAY 407
#define VARSTRING_MONST 408
#define VARSTRING_MONST_ARRAY 409
#define VARSTRING_OBJ 410
#define VARSTRING_OBJ_ARRAY 411
#define VARSTRING_SEL 412
#define VARSTRING_SEL_ARRAY 413
#define METHOD_INT 414
#define METHOD_INT_ARRAY 415
#define METHOD_STRING 416
#define METHOD_STRING_ARRAY 417
#define METHOD_VAR 418
#define METHOD_VAR_ARRAY 419
#define METHOD_COORD 420
#define METHOD_COORD_ARRAY 421
#define METHOD_REGION 422
#define METHOD_REGION_ARRAY 423
#define METHOD_MAPCHAR 424
#define METHOD_MAPCHAR_ARRAY 425
#define METHOD_MONST 426
#define METHOD_MONST_ARRAY 427
#define METHOD_OBJ 428
#define METHOD_OBJ_ARRAY 429
#define METHOD_SEL 430
#define METHOD_SEL_ARRAY 431
#define DICE 432

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
