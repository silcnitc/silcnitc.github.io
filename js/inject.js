let USE_TRANSPARENT_INVERSION_HEURISTIC = true;
let CACHE_TRANSPARENCY_TEST = true;
let PAGE_BRIGHTNESS = 0.7;

/*
 Elements with image data with a width and height both larger than
 ICON_THRESHOLD will be inverted regardless of their contents. Smaller images
 will still be inverted if they are transparent. If their transparency cannot be
 determined, they will still (still) be inverted if they are larger than
 ICON_THRESHOLD_1.

 The primary purpose of ICON_THRESHOLD is to minimizee the network and compute
 needed to determine the existence of alpha pixels for large images. The primary
 purpose of ICON_THRESHOLD_1 is to provide a good heuristic when we can't
 determine whether alpha pixels exist.
 */
let ICON_THRESHOLD = 100;
let ICON_THRESHOLD_1 = 40;

// Inject a <style> tag with the given CSS string.
function injectCSS(cssText) {
  let cssElement = document.createElement("style");
  cssElement.type = "text/css";
  cssElement.innerText = cssText;
  document.body.appendChild(cssElement);
}

// Inject a style sheet that adds CSS that naively inverting a page and then
// uninverts <img>, <canvas>, and <video> elements.
function darkifyPage() {
  let css = "";
  css += "html { background-color: black; }";
  css += `body::-webkit-scrollbar {
  width: 5px;               /* width of the entire scrollbar */
}

body::-webkit-scrollbar-track {
  background: darkbrown;        /* color of the tracking area */
}

body::-webkit-scrollbar-thumb {
  background-color: #f9f9f9;    /* color of the scroll thumb */
  border-radius: 10px;       /* roundness of the scroll thumb */
}`;
  css +=
    "body { filter: invert(100%) hue-rotate(180deg) brightness(" +
    PAGE_BRIGHTNESS +
    "); }";
  if (!USE_TRANSPARENT_INVERSION_HEURISTIC) {
    css += "IMG { filter: invert(100%) hue-rotate(180deg); }";
  }
  css += "CANVAS { filter: invert(100%) hue-rotate(180deg); }";
  css += "VIDEO { filter: invert(100%) hue-rotate(180deg); }";
  if (window.location.href.indexOf("docs.google.com/spreadsheets") > -1) {
    css +=
      "#waffle-grid-container { filter: invert(100%) hue-rotate(180deg); }";
  } else if (window.location.host == "www.youtube.com") {
    css += "#movie_player { background-color: white !important; }";
  }
  injectCSS(css);
}

/********** Simple Logic **********/
/*
 * All the code below this point has three functions:
 *   1) It hides the page with a div before it loads to prevent "flashing".
 *   2) It decides whether to call darkifyPage() based on the white/blacklist.
 *   3) It uninverts some nodes (iFrames and nodes with background images)
 */

// Cover the page in a dark <div> to prevent white flashing during page
// navigation.
function coverPage() {
  if (document.body) {
    coverObserver.disconnect();
    let cover = document.createElement("div");
    cover.id = "loremFishesPear";
    cover.style.position = "fixed";
    cover.style.left = 0;
    cover.style.top = 0;
    cover.style.zIndex = 10000;
    cover.style.width = "100vw";
    cover.style.height = "100vh";
    cover.style.backgroundColor = "#222";
    document.body.appendChild(cover);
  }
}

// Remove the <div> added by `coverPage()`.
function uncoverPage() {
  document.body.removeChild(document.getElementById("loremFishesPear"));
}

// We use this observer to try and show the cover the <div> referenced by
// `coverPage()` and `uncoverPage()` as quickly as possible.
let coverObserver = new MutationObserver(() => {
  coverPage();
});
coverObserver.observe(document.documentElement, { childList: true });

// Note: Chrome does not support "DOMAttrModified" devents, so we need to use
// mutation observers.

function listenForChanges(element) {
  if (element.tagName == "IMG") {
    let observer = new MutationObserver((mutations) => {
      mutations.forEach((mutation) => {
        if (mutation.attributeName == "src") {
          recursivelyApplyToDom(uninvert_smartly, mutation.target);
        }
      });
    });
    observer.observe(element, { attributes: true, attributeFilter: ["srcs"] });
  } else {
    let observer = new MutationObserver((mutations) => {
      mutations.forEach((mutation) => {
        if (mutation.attributeName == "style") {
          // The change could be a change to background-image, a change from
          // display:none, or neither.
          // In the second case, we need to recurse.
          recursivelyApplyToDom(uninvert_smartly, mutation.target);
        }
      });
    });
    observer.observe(element, { attributes: true, attributeFilter: ["style"] });
  }
}

let isLoaded = false;
let theme = localStorage.getItem("theme");
let shouldInvert = 0;
if (theme === "dark") {
  shouldInvert = 1;
} else {
  shouldInvert = 0;
}

window.addEventListener("load", () => {
  window.addEventListener("keydown", (event) => {
    if (event.keyCode == 65 && event.metaKey && event.altKey) {
      recursivelyApplyToDom(uninvert_smartly);
    }
  });
  isLoaded = true;
  if (shouldInvert === 1) {
    // Inject css tag to dark-mode the page.
    darkifyPage();

    // After injecting the CSS above, the next step is to call
    // `uninvert_smartly` on everything.
    recursivelyApplyToDom(uninvert_smartly);

    // Call `uninvert_smartly` on every new element and any time <img>.src changes.

    // Call `uninvert_smartly` on every new element.
    let observer = new MutationObserver((mutations) => {
      mutations.forEach((mutation) => {
        for (let node of mutation.addedNodes) {
          recursivelyApplyToDom(listenForChanges, node);
          recursivelyApplyToDom(uninvert_smartly, node);
        }
      });
    });
    // Set up observer to un-invert some nodes as they're created.
    observer.observe(document.body, { childList: true, subtree: true });
  }
  // When (actually before) the page loaded we covered it with a
  // div to prevent the "flashbang" effect where a page is
  // temporarily pure white.  We need to delete this div now (so
  // the user can see the page).
  uncoverPage();
});

// Check whether `string` fits the given `pattern`.
// For instance the string "dafaewFOOBARfeawfe" fits the pattern "*foobar*".
// function wildcardStringMatch(pattern, string) {
//   if (pattern == "") {
//     return false;
//   }
//   let patternArr = pattern.split("*");
//   for (let i = 0; i < patternArr.length; ++i) {
//     let index = string.indexOf(patternArr[i]);
//     if (index === -1) {
//       return false;
//     }
//     string = string.substr(index + patternArr[i].length);
//   }
//   return true;
// }

// Recursively apply `fn` to `node` and all of its descendants. If no `node` is
// given, apply to all nodes.
function recursivelyApplyToDom(fn, node) {
  if (node === undefined) {
    node = document.body;
  }
  fn(node);
  if (!node.children) return;
  for (let child of node.children) {
    recursivelyApplyToDom(fn, child);
  }
}

// Uninvert <div> elements that have a background image, or that are iFrames.
function uninvert_smartly(node) {
  if (node.nodeName === "IFRAME") {
    node.style.filter = "invert(100%) hue-rotate(180deg)";
    return;
  }

  let style;
  try {
    style = window.getComputedStyle(node);
  } catch {
    return;
  }

  if (USE_TRANSPARENT_INVERSION_HEURISTIC) {
    if (node.nodeName == "IMG") {
      asyncMaybeInvertImage(node, node.src);
    } else {
      let backgroundImageSource = null;
      if (style.getPropertyValue("background-image").includes("url")) {
        backgroundImageSource = style.getPropertyValue("background-image");
      } else if (style.getPropertyValue("background").includes("url")) {
        backgroundImageSource = style.getPropertyValue("background");
      }
      if (backgroundImageSource != null) {
        let urlStart = backgroundImageSource.indexOf('"') + 1;
        let urlEnd = backgroundImageSource.lastIndexOf('"') - 1;
        backgroundImageSource = backgroundImageSource.substr(
          urlStart,
          urlEnd - urlStart + 1
        );
        asyncMaybeInvertImage(node, backgroundImageSource);
      }
    }
  } else {
    let hasBackgroundImageSource = false;
    hasBackgroundImageSource |= style
      .getPropertyValue("background-image")
      .includes("url");
    hasBackgroundImageSource |= style
      .getPropertyValue("background")
      .includes("url");
    if (hasBackgroundImageSource) {
      // Only invert sufficiently large images. Small ones are probably icons.
      let rect = node.getBoundingClientRect();
      let body = document.body.getBoundingClientRect();
      if (rect.width > ICON_THRESHOLD) {
        node.style.filter = "invert(100%) hue-rotate(180deg)";
      }
    }
  }
}

function asyncMaybeInvertImage(element, url) {
  // TODO: Figure out why this is neccessary.
  // I *think* we're calling this before the element achieves its final size.
  setTimeout(() => {
    maybeInvertImage(element, url);
  }, 1);
  setTimeout(() => {
    maybeInvertImage(element, url);
  }, 10);
  setTimeout(() => {
    maybeInvertImage(element, url);
  }, 100);
  setTimeout(() => {
    maybeInvertImage(element, url);
  }, 1000);
}

// Only call if `USE_TRANSPARENT_INVERSION_HEURISTIC` is true.
// Of all the bugs I've seen, they've all been rectified by calling this function.
// In other words, this extension's only problem is that it sometimes doesn't
// call this function on <img> tags and on elements with image backgrounds.
// In other other words, the problem is with the MutationObserver code.
function maybeInvertImage(element, url) {
  let rect = element.getBoundingClientRect();
  if (Math.min(rect.width, rect.height) >= ICON_THRESHOLD) {
    element.style.filter = "invert(100%) hue-rotate(180deg)";
    return;
  }
  imageTransparentAtURL(url, (url, isTransparent) => {
    if (isTransparent === true) {
      element.style.filter = "";
    } else if (isTransparent === false) {
      element.style.filter = "invert(100%) hue-rotate(180deg)";
    } else {
      // There was a cross-origin issue, so we can't determine if the image is
      // transparent. Therefore, we use the heuristic that small images
      // shouldn't be uninverted.
      if (Math.min(rect.width, rect.height) >= ICON_THRESHOLD_1) {
        element.style.filter = "invert(100%) hue-rotate(180deg)";
      } else {
        element.style.filter = "";
      }
    }
  });
}

// maybeInvertImage(document.getElementById(":2r_1-e"), document.getElementById(":2r_1-e").src)

// Calls `callback(url, result)` either synchronously or asynchronously.
// `result` may take one of three values:
//   * true - the image has at least one translucent pixel
//   * false - the image has only opaque pixels
//   * undefined - an error occured (probably cross origin in nature)
function imageTransparentAtURL(url, callback) {
  if (url.startsWith("https")) {
    callback(url, undefined);
  }
  if (!this._cache) {
    this._cache = {};
  }
  if (CACHE_TRANSPARENCY_TEST) {
    if (url in this._cache) {
      callback(this._cache[url], url);
    }
  }
  let image = new Image();
  image.crossOrigin = "Anonymous";
  image.onload = () => {
    let isTransparent = isLoadedImageTransparent(image);
    if (CACHE_TRANSPARENCY_TEST) {
      this._cache[url] = isTransparent;
    }
    callback(url, isTransparent);
  };
  image.onerror = () => {
    if (CACHE_TRANSPARENCY_TEST) {
      this._cache[url] = undefined;
    }
    callback(url, undefined);
  };
  image.src = url;
}

// A synchronous version of `imageTransparentAtURL` for when the image is
// already loaded.
function isLoadedImageTransparent(image) {
  if (!isLoadedImageTransparent._canvas) {
    isLoadedImageTransparent._canvas = document.createElement("canvas");
    isLoadedImageTransparent._context = isLoadedImageTransparent._canvas.getContext(
      "2d"
    );
  }
  isLoadedImageTransparent._canvas.width = image.width;
  isLoadedImageTransparent._canvas.height = image.height;
  isLoadedImageTransparent._context.drawImage(image, 0, 0);
  let data;
  try {
    data = isLoadedImageTransparent._context.getImageData(
      0,
      0,
      isLoadedImageTransparent._canvas.width,
      isLoadedImageTransparent._canvas.height
    ).data;
  } catch {
    // An error occured fetching the image's pixel data. This is probably due to
    // cross origin restrictions.
    return undefined;
  }
  for (let i = 3; i < data.length; i += 4) {
    if (data[i] < 255) return true;
  }
  return false;
}

// Checks if any of the given suffixes ends the given string.
// function endsWithAny(string, suffixes) {
//   for (let suffix of suffixes) {
//     if (string.endsWith(suffix)) {
//       return true;
//     }
//   }
//   return false;
// }

// `shouldInvert` resolves to -1 or +1 eventually.
// let shouldInvert = 0;
// chrome.storage.sync.get("darkify_blacklist", (result) => {
//   let blacklist = result["darkify_blacklist"];
//   if (blacklist === undefined) {
//     let prepopulatedBlacklist = [
//       "https://www.netflix.com/*",
//       "https://www.disneyplus.com/*",
//       "https://twitter.com/*",
//     ];
//     chrome.storage.sync.set(
//       { darkify_blacklist: prepopulatedBlacklist },
//       (x) => {}
//     );
//     blacklist = [];
//   }
//   let pageUrl = window.location.href;
//   if (
//     endsWithAny(pageUrl.toLowerCase(), [
//       ".gif",
//       ".jpeg",
//       "jpg",
//       "mp4",
//       ".png",
//       ".webm",
//       ".webp",
//     ])
//   ) {
//     return;
//   }
//   for (let i = 0; i < blacklist.length; ++i) {
//     if (wildcardStringMatch(blacklist[i], pageUrl)) {
//       shouldInvert = -1;
//       return;
//     }
//   }
//   if (window.location.href.endsWith(".pdf")) {
//     shouldInvert = -1;
//     // TODO: Find if there is a non-bad solution.s
//     // injectCSS(`embed { filter: invert(1) hue-rotate(180deg); }`);
//     return;
//   }
//   shouldInvert = 1;
//   if (isLoaded) {
//     darkifyPage();
//   }
// });
