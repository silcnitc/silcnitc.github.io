function()
{
  if(typeof(window.getComputedStyle) == 'undefined')
  {
    return;
  }
  var pre = document.getElementsByTagName('pre');
  for(var len = pre.length, i = 0; i < len; i ++)
  {
    var code = pre[i].getElementsByTagName('code').item(0);
    if(!code)
    {
      code = pre[i].getElementsByTagName('samp').item(0);
      if(!code)
      {
        continue;
      }
    }
    var column = document.createElement('div');
    column.setAttribute('aria-hidden', 'true');
    for(var n = 0; n < code.innerHTML.split(/[nr]/g).length; n ++)
    {
      column.appendChild(document.createElement('span'));
    }
    pre[i].insertBefore(column, code);
    pre[i].className = 'line-numbers';
  }
})();
