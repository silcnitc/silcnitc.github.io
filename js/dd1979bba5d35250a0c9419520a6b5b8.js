document.write('<link rel="stylesheet" href="https://assets-cdn.github.com/assets/gist-embed-a3faa5f87160a19edd114078e53e5c095f4fc2a8ced2f5f10947ba7b66179548.css">')
document.write('<div id=\"gist44725228\" class=\"gist\">\n    <div class=\"gist-file\">\n      <div class=\"gist-data\">\n        <div class=\"js-gist-file-update-container js-task-list-container file-box\">\n  <div id=\"file-nodestructure-h\" class=\"file\">\n    \n\n  <div itemprop=\"text\" class=\"blob-wrapper data type-c\">\n      <table class=\"highlight tab-size js-file-line-container\" data-tab-size=\"8\">\n      <tr>\n        <td id=\"file-nodestructure-h-L1\" class=\"blob-num js-line-number\" data-line-number=\"1\"><\/td>\n        <td id=\"file-nodestructure-h-LC1\" class=\"blob-code blob-code-inner js-file-line\"><span class=\"pl-k\">typedef<\/span> <span class=\"pl-k\">struct<\/span> tnode{<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-nodestructure-h-L2\" class=\"blob-num js-line-number\" data-line-number=\"2\"><\/td>\n        <td id=\"file-nodestructure-h-LC2\" class=\"blob-code blob-code-inner js-file-line\">	<span class=\"pl-k\">int<\/span> val;			<span class=\"pl-c\"><span class=\"pl-c\">//<\/span>value of for the expression tree<\/span><\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-nodestructure-h-L3\" class=\"blob-num js-line-number\" data-line-number=\"3\"><\/td>\n        <td id=\"file-nodestructure-h-LC3\" class=\"blob-code blob-code-inner js-file-line\">	<span class=\"pl-k\">char<\/span>* varname;			<span class=\"pl-c\"><span class=\"pl-c\">//<\/span>name of the variable a-z.<\/span><\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-nodestructure-h-L4\" class=\"blob-num js-line-number\" data-line-number=\"4\"><\/td>\n        <td id=\"file-nodestructure-h-LC4\" class=\"blob-code blob-code-inner js-file-line\">	<span class=\"pl-k\">int<\/span> nodetype;			<span class=\"pl-c\"><span class=\"pl-c\">//<\/span>node type information,eg : NODETYPE_READ,NODETYPE_PLUS,NODETYPE_WRITE etc<\/span><\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-nodestructure-h-L5\" class=\"blob-num js-line-number\" data-line-number=\"5\"><\/td>\n        <td id=\"file-nodestructure-h-LC5\" class=\"blob-code blob-code-inner js-file-line\">	<span class=\"pl-k\">struct<\/span> tnode *left,*right;	<span class=\"pl-c\"><span class=\"pl-c\">//<\/span>left and right branches<\/span><\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-nodestructure-h-L6\" class=\"blob-num js-line-number\" data-line-number=\"6\"><\/td>\n        <td id=\"file-nodestructure-h-LC6\" class=\"blob-code blob-code-inner js-file-line\">}tnode;<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-nodestructure-h-L7\" class=\"blob-num js-line-number\" data-line-number=\"7\"><\/td>\n        <td id=\"file-nodestructure-h-LC7\" class=\"blob-code blob-code-inner js-file-line\">\n<\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-nodestructure-h-L8\" class=\"blob-num js-line-number\" data-line-number=\"8\"><\/td>\n        <td id=\"file-nodestructure-h-LC8\" class=\"blob-code blob-code-inner js-file-line\"><span class=\"pl-c\"><span class=\"pl-c\">/*<\/span>Create a node tnode<span class=\"pl-c\">*/<\/span><\/span><\/td>\n      <\/tr>\n      <tr>\n        <td id=\"file-nodestructure-h-L9\" class=\"blob-num js-line-number\" data-line-number=\"9\"><\/td>\n        <td id=\"file-nodestructure-h-LC9\" class=\"blob-code blob-code-inner js-file-line\"><span class=\"pl-k\">struct<\/span> tnode* <span class=\"pl-en\">createTree<\/span>(<span class=\"pl-k\">int<\/span> val, <span class=\"pl-k\">char<\/span>* c, <span class=\"pl-k\">int<\/span> nodetype, <span class=\"pl-k\">struct<\/span> tnode *l, <span class=\"pl-k\">struct<\/span> tnode *r);<\/td>\n      <\/tr>\n<\/table>\n\n  <\/div>\n\n  <\/div>\n  \n<\/div>\n\n      <\/div>\n      <div class=\"gist-meta\">\n        <a href=\"https://gist.github.com/dattathallam/dd1979bba5d35250a0c9419520a6b5b8/raw/43e1221bd7c88ca1d4722bfc313faf95ab022f8a/nodestructure.h\" style=\"float:right\">view raw<\/a>\n        <a href=\"https://gist.github.com/dattathallam/dd1979bba5d35250a0c9419520a6b5b8#file-nodestructure-h\">nodestructure.h<\/a>\n        hosted with &#10084; by <a href=\"https://github.com\">GitHub<\/a>\n      <\/div>\n    <\/div>\n<\/div>\n')
