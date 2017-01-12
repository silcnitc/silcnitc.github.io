document.write('<link rel="stylesheet" href="https://assets-cdn.github.com/assets/gist-embed-51ae8299bd14b6c42fb20b0ce07f643175347abaca051f9c55e114bfc98b10b1.css">')
document.write('<div id=\"gist32167335\" class=\"gist\">\n    <div class=\"gist-file\">\n      <div class=\"gist-data\">\n        <div class=\"js-gist-file-update-container js-task-list-container file-box\">\n  <div id=\"file-grammar_outline_fdefblock-c\" class=\"file\">\n    \n\n  <div itemprop=\"text\" class=\"blob-wrapper data type-c\">\n      <table class=\"highlight tab-size js-file-line-container\" data-tab-size=\"8\">\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L1\" class=\"blob-num js-line-number\" data-line-number=\"1\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC1\" class=\"blob-code blob-code-inner js-file-line\">FDefBlock : TypeName ID <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> ArgList <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span> <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>{<span class=\"pl-pds\">&#39;<\/span><\/span> LdeclBlock Body <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>}<span class=\"pl-pds\">&#39;<\/span><\/span>  { <span class=\"pl-c1\"><a href=""> validate</a><\/span>($<span class=\"pl-c1\">2<\/span>-&gt;name,$<span class=\"pl-c1\">1<\/span>,$<span class=\"pl-c1\">4<\/span>,$<span class=\"pl-c1\">7<\/span>,$<span class=\"pl-c1\">8<\/span>); }                                <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L2\" class=\"blob-num js-line-number\" data-line-number=\"2\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC2\" class=\"blob-code blob-code-inner js-file-line\">          ;<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L3\" class=\"blob-num js-line-number\" data-line-number=\"3\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC3\" class=\"blob-code blob-code-inner js-file-line\">\n<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L4\" class=\"blob-num js-line-number\" data-line-number=\"4\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC4\" class=\"blob-code blob-code-inner js-file-line\">Body      : BEGIN Slist Retstmt END     <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L5\" class=\"blob-num js-line-number\" data-line-number=\"5\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC5\" class=\"blob-code blob-code-inner js-file-line\">          ;<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L4\" class=\"blob-num js-line-number\" data-line-number=\"4\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC4\" class=\"blob-code blob-code-inner js-file-line\">Retstmt   : RETURN Expr \';\'    <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L5\" class=\"blob-num js-line-number\" data-line-number=\"5\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC5\" class=\"blob-code blob-code-inner js-file-line\">          ;<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L6\" class=\"blob-num js-line-number\" data-line-number=\"6\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC6\" class=\"blob-code blob-code-inner js-file-line\">\n<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L7\" class=\"blob-num js-line-number\" data-line-number=\"7\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC7\" class=\"blob-code blob-code-inner js-file-line\">Slist     : Slist Stmt  <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L8\" class=\"blob-num js-line-number\" data-line-number=\"8\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC8\" class=\"blob-code blob-code-inner js-file-line\">          |             <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L9\" class=\"blob-num js-line-number\" data-line-number=\"9\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC9\" class=\"blob-code blob-code-inner js-file-line\">          ;<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L10\" class=\"blob-num js-line-number\" data-line-number=\"10\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC10\" class=\"blob-code blob-code-inner js-file-line\">\n<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L11\" class=\"blob-num js-line-number\" data-line-number=\"11\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC11\" class=\"blob-code blob-code-inner js-file-line\">Stmt      : ID ASGN Expr <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>;<span class=\"pl-pds\">&#39;<\/span><\/span> <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L12\" class=\"blob-num js-line-number\" data-line-number=\"12\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC12\" class=\"blob-code blob-code-inner js-file-line\">          | ....<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L13\" class=\"blob-num js-line-number\" data-line-number=\"13\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC13\" class=\"blob-code blob-code-inner js-file-line\">          | IF <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> Expr <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span> THEN Slist ELSE Slist ENDIF <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L14\" class=\"blob-num js-line-number\" data-line-number=\"14\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC14\" class=\"blob-code blob-code-inner js-file-line\">          | ...<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L15\" class=\"blob-num js-line-number\" data-line-number=\"15\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC15\" class=\"blob-code blob-code-inner js-file-line\">          | ID ASGN ALLOC<span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span>     <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L16\" class=\"blob-num js-line-number\" data-line-number=\"16\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC16\" class=\"blob-code blob-code-inner js-file-line\">          | FIELD ASGN ALLOC<span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span>   <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L17\" class=\"blob-num js-line-number\" data-line-number=\"17\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC17\" class=\"blob-code blob-code-inner js-file-line\">          | FREE <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> ID <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span>           <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L18\" class=\"blob-num js-line-number\" data-line-number=\"18\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC18\" class=\"blob-code blob-code-inner js-file-line\">          | FREE <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> FIELD <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span>  <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L19\" class=\"blob-num js-line-number\" data-line-number=\"19\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC19\" class=\"blob-code blob-code-inner js-file-line\">          | READ <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> ID <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span><\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L20\" class=\"blob-num js-line-number\" data-line-number=\"20\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC20\" class=\"blob-code blob-code-inner js-file-line\">          | READ <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> FIELD <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span><\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L21\" class=\"blob-num js-line-number\" data-line-number=\"21\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC21\" class=\"blob-code blob-code-inner js-file-line\">          | WRITE <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> EXPR <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span><\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L22\" class=\"blob-num js-line-number\" data-line-number=\"22\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC22\" class=\"blob-code blob-code-inner js-file-line\">          ;<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L23\" class=\"blob-num js-line-number\" data-line-number=\"23\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC23\" class=\"blob-code blob-code-inner js-file-line\">\n<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L24\" class=\"blob-num js-line-number\" data-line-number=\"24\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC24\" class=\"blob-code blob-code-inner js-file-line\">FIELD     : ID <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>.<span class=\"pl-pds\">&#39;<\/span><\/span> ID   <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L25\" class=\"blob-num js-line-number\" data-line-number=\"25\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC25\" class=\"blob-code blob-code-inner js-file-line\">          | FIELD <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>.<span class=\"pl-pds\">&#39;<\/span><\/span> ID  <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L26\" class=\"blob-num js-line-number\" data-line-number=\"26\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC26\" class=\"blob-code blob-code-inner js-file-line\">          ;<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L27\" class=\"blob-num js-line-number\" data-line-number=\"27\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC27\" class=\"blob-code blob-code-inner js-file-line\">\n<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L28\" class=\"blob-num js-line-number\" data-line-number=\"28\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC28\" class=\"blob-code blob-code-inner js-file-line\">Expr      : Expr PLUS Expr   { $$ = <span class=\"pl-c1\"><a href="data_structures/abstract-syntax-tree.html#nav-associated-methods">TreeCreate</a><\/span>(<span class=\"pl-c1\"><a href="data_structures/type-table.html#nav-associated-methods">TLookup</a><\/span>(<span class=\"pl-s\"><span class=\"pl-pds\">&quot;<\/span>int<span class=\"pl-pds\">&quot;<\/span><\/span>),NODETYPE_PLUS,<span class=\"pl-c1\">NULL<\/span>,(<span class=\"pl-k\">union<\/span> Constant){},<span class=\"pl-c1\">NULL<\/span>,$<span class=\"pl-c1\">1<\/span>,$<span class=\"pl-c1\">3<\/span>,<span class=\"pl-c1\">NULL<\/span>); }<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L29\" class=\"blob-num js-line-number\" data-line-number=\"29\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC29\" class=\"blob-code blob-code-inner js-file-line\">          | ....<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L30\" class=\"blob-num js-line-number\" data-line-number=\"30\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC30\" class=\"blob-code blob-code-inner js-file-line\">          | <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> Expr <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span>      <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L31\" class=\"blob-num js-line-number\" data-line-number=\"31\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC31\" class=\"blob-code blob-code-inner js-file-line\">          | NUM               <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L32\" class=\"blob-num js-line-number\" data-line-number=\"32\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC32\" class=\"blob-code blob-code-inner js-file-line\">          | ID                <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L33\" class=\"blob-num js-line-number\" data-line-number=\"33\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC33\" class=\"blob-code blob-code-inner js-file-line\">          | ID <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>[<span class=\"pl-pds\">&#39;<\/span><\/span> Expr <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>]<span class=\"pl-pds\">&#39;<\/span><\/span>  <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L34\" class=\"blob-num js-line-number\" data-line-number=\"34\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC34\" class=\"blob-code blob-code-inner js-file-line\">          | FIELD            <\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L35\" class=\"blob-num js-line-number\" data-line-number=\"35\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC35\" class=\"blob-code blob-code-inner js-file-line\">          | ID <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>(<span class=\"pl-pds\">&#39;<\/span><\/span> Param <span class=\"pl-s\"><span class=\"pl-pds\">&#39;<\/span>)<span class=\"pl-pds\">&#39;<\/span><\/span>  {<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L36\" class=\"blob-num js-line-number\" data-line-number=\"36\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC36\" class=\"blob-code blob-code-inner js-file-line\">                                gtemp = <span class=\"pl-c1\"><a href="data_structures/global-symbol-table.html#nav-associated-methods">GLookup</a><\/span>($<span class=\"pl-c1\">1<\/span>-&gt;name);<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L37\" class=\"blob-num js-line-number\" data-line-number=\"37\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC37\" class=\"blob-code blob-code-inner js-file-line\">                                <span class=\"pl-k\">if<\/span>(gtemp == <span class=\"pl-c1\">NULL<\/span>){<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L38\" class=\"blob-num js-line-number\" data-line-number=\"38\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC38\" class=\"blob-code blob-code-inner js-file-line\">                                	<span class=\"pl-c1\">yyerror<\/span>(<span class=\"pl-s\"><span class=\"pl-pds\">&quot;<\/span>Yacc : Undefined function<span class=\"pl-pds\">&quot;<\/span><\/span>);<span class=\"pl-c1\">exit<\/span>(<span class=\"pl-c1\">1<\/span>);<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L39\" class=\"blob-num js-line-number\" data-line-number=\"39\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC39\" class=\"blob-code blob-code-inner js-file-line\">                                }<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L40\" class=\"blob-num js-line-number\" data-line-number=\"40\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC40\" class=\"blob-code blob-code-inner js-file-line\">                                $$ = <span class=\"pl-c1\"><a href="data_structures/abstract-syntax-tree.html#nav-associated-methods">TreeCreate</a><\/span>(gtemp-&gt;type,NODETYPE_FUNCTION,$<span class=\"pl-c1\">1<\/span>-&gt;name,(<span class=\"pl-k\">union<\/span> Constant){},$<span class=\"pl-c1\">3<\/span>,<span class=\"pl-c1\">NULL<\/span>,<span class=\"pl-c1\">NULL<\/span>,<span class=\"pl-c1\">NULL<\/span>);<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L41\" class=\"blob-num js-line-number\" data-line-number=\"41\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC41\" class=\"blob-code blob-code-inner js-file-line\">                                $$-&gt;Gentry = gtemp;<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L42\" class=\"blob-num js-line-number\" data-line-number=\"42\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC42\" class=\"blob-code blob-code-inner js-file-line\">                              }<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-grammar_outline_fdefblock-c-L43\" class=\"blob-num js-line-number\" data-line-number=\"43\"><\/td>\n        <td id=\"file-grammar_outline_fdefblock-c-LC43\" class=\"blob-code blob-code-inner js-file-line\">          ;<\/td>\n      <\/tr><tr>\n        <td id=\"file-grammar_outline_mainblock-c-L44\" class=\"blob-num js-line-number\" data-line-number=\"44\"><\/td>\n<td id=\"file-grammar_outline_mainblock-c-LC44\" class=\"blob-code blob-code-inner js-file-line\">  <span class=\"pl-c\">//NOTE: The function validate(char *name,struct Typetable *type, struct Argstruct *arg, struct Lsymbol *l, struct ASTNode *a)  <\/br>must check for <b>name equivalence<\/b> between the argument types and names in the function declaration and the function definition.  <\/br>Name equivalence requires that types and names of each argument in the definition must match exactly with the types and names <\/br>of each argument in the declaration.  Details of implementation of this function are left to you and not specified in the documentation.  <\/span><\/td>\n      <\/tr>\n\n<\/table>\n\n  <\/div>\n\n  <\/div>\n  \n<\/div>\n\n      <\/div>\n      <div class=\"gist-meta\">\n        <a href=\"https://gist.github.com/vishnupriyam/eaa002504dcd72a87d1e/raw/eb2a2cb5277edb35037d9d3f15202508e9bd28ba/Grammar_outline_FDefBlock.c\" style=\"float:right\">view raw<\/a>\n        <a href=\"https://gist.github.com/vishnupriyam/eaa002504dcd72a87d1e#file-grammar_outline_fdefblock-c\">Grammar_outline_FDefBlock<\/a>\n        hosted with &#10084; by <a href=\"https://github.com\">GitHub<\/a>\n      <\/div>\n    <\/div>\n<\/div>\n')
