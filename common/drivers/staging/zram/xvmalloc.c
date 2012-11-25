  


<!DOCTYPE html>
<html>
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# githubog: http://ogp.me/ns/fb/githubog#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title>Kernel-Nexus7/drivers/staging/zram/xvmalloc.c at master · motley-git/Kernel-Nexus7</title>
    <link rel="search" type="application/opensearchdescription+xml" href="/opensearch.xml" title="GitHub" />
    <link rel="fluid-icon" href="https://github.com/fluidicon.png" title="GitHub" />
    <link rel="apple-touch-icon-precomposed" sizes="57x57" href="apple-touch-icon-114.png" />
    <link rel="apple-touch-icon-precomposed" sizes="114x114" href="apple-touch-icon-114.png" />
    <link rel="apple-touch-icon-precomposed" sizes="72x72" href="apple-touch-icon-144.png" />
    <link rel="apple-touch-icon-precomposed" sizes="144x144" href="apple-touch-icon-144.png" />
    <meta name="msapplication-TileImage" content="/windows-tile.png">
    <meta name="msapplication-TileColor" content="#ffffff">

    
    
    <link rel="icon" type="image/x-icon" href="/favicon.ico" />

    <meta content="authenticity_token" name="csrf-param" />
<meta content="2cRprTdeGeMej+y4qCrR6po2DjANZEtj0rDCbpPqWSo=" name="csrf-token" />

    <link href="https://a248.e.akamai.net/assets.github.com/assets/github-dd95960936fd09bc718d54eac1f1683ed3376505.css" media="screen" rel="stylesheet" type="text/css" />
    <link href="https://a248.e.akamai.net/assets.github.com/assets/github2-7fc9083d70adfe618db229248dd342e4a50999a6.css" media="screen" rel="stylesheet" type="text/css" />
    


    <script src="https://a248.e.akamai.net/assets.github.com/assets/frameworks-57542e0cba19d068168099f287c117efa142863c.js" type="text/javascript"></script>
    <script src="https://a248.e.akamai.net/assets.github.com/assets/github-6f3bd550f3d93d807bbc14d8cfce5a5a393cf3f7.js" type="text/javascript"></script>
    

      <link rel='permalink' href='/motley-git/Kernel-Nexus7/blob/02eea732fc1f834d620eba4a5a21785fd913ab71/drivers/staging/zram/xvmalloc.c'>
    <meta property="og:title" content="Kernel-Nexus7"/>
    <meta property="og:type" content="githubog:gitrepository"/>
    <meta property="og:url" content="https://github.com/motley-git/Kernel-Nexus7"/>
    <meta property="og:image" content="https://a248.e.akamai.net/assets.github.com/images/gravatars/gravatar-user-420.png?1345673561"/>
    <meta property="og:site_name" content="GitHub"/>
    <meta property="og:description" content="motley kernel for Nexus7 (Grouper). Contribute to Kernel-Nexus7 development by creating an account on GitHub."/>

    <meta name="description" content="motley kernel for Nexus7 (Grouper). Contribute to Kernel-Nexus7 development by creating an account on GitHub." />

  <link href="https://github.com/motley-git/Kernel-Nexus7/commits/master.atom" rel="alternate" title="Recent Commits to Kernel-Nexus7:master" type="application/atom+xml" />

  </head>


  <body class="logged_in page-blob linux vis-public env-production ">
    <div id="wrapper">

      

      

      


        <div class="header header-logged-in true">
          <div class="container clearfix">

            <a class="header-logo-blacktocat" href="https://github.com/">
  <span class="mega-icon mega-icon-blacktocat"></span>
</a>

            <div class="divider-vertical"></div>

            
  <a href="/notifications" class="notification-indicator tooltipped downwards" title="You have unread notifications">
    <span class="mail-status unread"></span>
  </a>
  <div class="divider-vertical"></div>


              
  <div class="topsearch command-bar-activated">
    <form accept-charset="UTF-8" action="/search" class="command_bar_form" id="top_search_form" method="get">
  <a href="/search/advanced" class="advanced-search tooltipped downwards command-bar-search" id="advanced_search" title="Advanced search"><span class="mini-icon mini-icon-advanced-search "></span></a>

  <input type="text" name="q" id="command-bar" placeholder="Search or type a command" tabindex="1" data-username="Whitexp" autocapitalize="off">

  <span class="mini-icon help tooltipped downwards" title="Show command bar help">
    <span class="mini-icon mini-icon-help"></span>
  </span>

  <input type="hidden" name="ref" value="commandbar">

  <div class="divider-vertical"></div>
</form>



    <ul class="top-nav">
        <li class="explore"><a href="https://github.com/explore">Explore</a></li>
        <li><a href="https://gist.github.com">Gist</a></li>
        <li><a href="/blog">Blog</a></li>
      <li><a href="http://help.github.com">Help</a></li>
    </ul>
  </div>


            

  
    <ul id="user-links">
      <li>
        <a href="https://github.com/Whitexp" class="name">
          <img height="20" src="https://secure.gravatar.com/avatar/f0c16ca5f32f7b38ca251e846b00c80b?s=140&amp;d=https://a248.e.akamai.net/assets.github.com%2Fimages%2Fgravatars%2Fgravatar-user-420.png" width="20" /> Whitexp
        </a>
      </li>
      <li>
        <a href="/new" id="new_repo" class="tooltipped downwards" title="Create a new repo">
          <span class="mini-icon mini-icon-create"></span>
        </a>
      </li>
      <li>
        <a href="/settings/profile" id="account_settings"
          class="tooltipped downwards"
          title="Account settings ">
          <span class="mini-icon mini-icon-account-settings"></span>
        </a>
      </li>
      <li>
          <a href="/logout" data-method="post" id="logout" class="tooltipped downwards" title="Sign out">
            <span class="mini-icon mini-icon-logout"></span>
          </a>
      </li>
    </ul>



            
          </div>
        </div>


      

      


            <div class="site hfeed" itemscope itemtype="http://schema.org/WebPage">
      <div class="hentry">
        
        <div class="pagehead repohead instapaper_ignore readability-menu">
          <div class="container">
            <div class="title-actions-bar">
              


                  <ul class="pagehead-actions">

          <li class="subscription">
              <form accept-charset="UTF-8" action="/notifications/subscribe" data-autosubmit="true" data-remote="true" method="post"><div style="margin:0;padding:0;display:inline"><input name="authenticity_token" type="hidden" value="2cRprTdeGeMej+y4qCrR6po2DjANZEtj0rDCbpPqWSo=" /></div>  <input id="repository_id" name="repository_id" type="hidden" value="5063150" />
  <div class="context-menu-container js-menu-container js-context-menu">
    <span class="minibutton switcher bigger js-menu-target">
      <span class="js-context-button">
          <span class="mini-icon mini-icon-watching"></span>Watch
      </span>
    </span>

    <div class="context-pane js-menu-content">
      <a href="javascript:;" class="close js-menu-close"><span class="mini-icon mini-icon-remove-close"></span></a>
      <div class="context-title">Notification status</div>

      <div class="context-body pane-selector">
        <ul class="js-navigation-container">
          <li class="selector-item js-navigation-item js-navigation-target selected">
            <span class="mini-icon mini-icon-confirm"></span>
            <label>
              <input checked="checked" id="do_included" name="do" type="radio" value="included" />
              <h4>Not watching</h4>
              <p>You will only receive notifications when you participate or are mentioned.</p>
            </label>
            <span class="context-button-text js-context-button-text">
              <span class="mini-icon mini-icon-watching"></span>
              Watch
            </span>
          </li>
          <li class="selector-item js-navigation-item js-navigation-target ">
            <span class="mini-icon mini-icon-confirm"></span>
            <label>
              <input id="do_subscribed" name="do" type="radio" value="subscribed" />
              <h4>Watching</h4>
              <p>You will receive all notifications for this repository.</p>
            </label>
            <span class="context-button-text js-context-button-text">
              <span class="mini-icon mini-icon-unwatch"></span>
              Unwatch
            </span>
          </li>
          <li class="selector-item js-navigation-item js-navigation-target ">
            <span class="mini-icon mini-icon-confirm"></span>
            <label>
              <input id="do_ignore" name="do" type="radio" value="ignore" />
              <h4>Ignored</h4>
              <p>You will not receive notifications for this repository.</p>
            </label>
            <span class="context-button-text js-context-button-text">
              <span class="mini-icon mini-icon-mute"></span>
              Stop ignoring
            </span>
          </li>
        </ul>
      </div>
    </div>
  </div>
</form>
          </li>

          <li class="js-toggler-container js-social-container starring-container ">
            <a href="/motley-git/Kernel-Nexus7/unstar" class="minibutton js-toggler-target starred" data-remote="true" data-method="post" rel="nofollow">
              <span class="mini-icon mini-icon-star"></span>Unstar
            </a><a href="/motley-git/Kernel-Nexus7/star" class="minibutton js-toggler-target unstarred" data-remote="true" data-method="post" rel="nofollow">
              <span class="mini-icon mini-icon-star"></span>Star
            </a><a class="social-count js-social-count" href="/motley-git/Kernel-Nexus7/stargazers">7</a>
          </li>

              <li>
                <a href="/motley-git/Kernel-Nexus7/fork_select" class="minibutton js-toggler-target lighter" rel="facebox nofollow"><span class="mini-icon mini-icon-fork"></span>Fork</a><a href="/motley-git/Kernel-Nexus7/network" class="social-count">2</a>
              </li>


    </ul>

              <h1 itemscope itemtype="http://data-vocabulary.org/Breadcrumb" class="entry-title public">
                <span class="repo-label"><span>public</span></span>
                <span class="mega-icon mega-icon-public-repo"></span>
                <span class="author vcard">
                  <a href="/motley-git" class="url fn" itemprop="url" rel="author">
                  <span itemprop="title">motley-git</span>
                  </a></span> /
                <strong><a href="/motley-git/Kernel-Nexus7" class="js-current-repository">Kernel-Nexus7</a></strong>
              </h1>
            </div>

            

  <ul class="tabs">
    <li><a href="/motley-git/Kernel-Nexus7" class="selected" highlight="repo_sourcerepo_downloadsrepo_commitsrepo_tagsrepo_branches">Code</a></li>
    <li><a href="/motley-git/Kernel-Nexus7/network" highlight="repo_network">Network</a></li>
    <li><a href="/motley-git/Kernel-Nexus7/pulls" highlight="repo_pulls">Pull Requests <span class='counter'>0</span></a></li>

      <li><a href="/motley-git/Kernel-Nexus7/issues" highlight="repo_issues">Issues <span class='counter'>0</span></a></li>

      <li><a href="/motley-git/Kernel-Nexus7/wiki" highlight="repo_wiki">Wiki</a></li>


    <li><a href="/motley-git/Kernel-Nexus7/graphs" highlight="repo_graphsrepo_contributors">Graphs</a></li>


  </ul>
  
<div class="tabnav">

  <span class="tabnav-right">
    <ul class="tabnav-tabs">
      <li><a href="/motley-git/Kernel-Nexus7/tags" class="tabnav-tab" highlight="repo_tags">Tags <span class="counter blank">0</span></a></li>
      <li><a href="/motley-git/Kernel-Nexus7/downloads" class="tabnav-tab" highlight="repo_downloads">Downloads <span class="counter blank">0</span></a></li>
    </ul>
    
  </span>

  <div class="tabnav-widget scope">


    <div class="context-menu-container js-menu-container js-context-menu">
      <a href="#"
         class="minibutton bigger switcher js-menu-target js-commitish-button btn-branch repo-tree"
         data-hotkey="w"
         data-ref="master">
         <span><em class="mini-icon mini-icon-branch"></em><i>branch:</i> master</span>
      </a>

      <div class="context-pane commitish-context js-menu-content">
        <a href="javascript:;" class="close js-menu-close"><span class="mini-icon mini-icon-remove-close"></span></a>
        <div class="context-title">Switch branches/tags</div>
        <div class="context-body pane-selector commitish-selector js-navigation-container">
          <div class="filterbar">
            <input type="text" id="context-commitish-filter-field" class="js-navigation-enable js-filterable-field" placeholder="Filter branches/tags">
            <ul class="tabs">
              <li><a href="#" data-filter="branches" class="selected">Branches</a></li>
                <li><a href="#" data-filter="tags">Tags</a></li>
            </ul>
          </div>

          <div class="js-filter-tab js-filter-branches">
            <div data-filterable-for="context-commitish-filter-field" data-filterable-type=substring>
                <div class="commitish-item branch-commitish selector-item js-navigation-item js-navigation-target selected">
                  <span class="mini-icon mini-icon-confirm"></span>
                  <h4>
                      <a href="/motley-git/Kernel-Nexus7/blob/master/drivers/staging/zram/xvmalloc.c" class="js-navigation-open" data-name="master" rel="nofollow">master</a>
                  </h4>
                </div>
                <div class="commitish-item branch-commitish selector-item js-navigation-item js-navigation-target ">
                  <span class="mini-icon mini-icon-confirm"></span>
                  <h4>
                      <a href="/motley-git/Kernel-Nexus7/blob/motley_v2/drivers/staging/zram/xvmalloc.c" class="js-navigation-open" data-name="motley_v2" rel="nofollow">motley_v2</a>
                  </h4>
                </div>
                <div class="commitish-item branch-commitish selector-item js-navigation-item js-navigation-target ">
                  <span class="mini-icon mini-icon-confirm"></span>
                  <h4>
                      <a href="/motley-git/Kernel-Nexus7/blob/motley_v3/drivers/staging/zram/xvmalloc.c" class="js-navigation-open" data-name="motley_v3" rel="nofollow">motley_v3</a>
                  </h4>
                </div>
            </div>
            <div class="no-results">Nothing to show</div>
          </div>

            <div class="js-filter-tab js-filter-tags filter-tab-empty" style="display:none">
              <div data-filterable-for="context-commitish-filter-field" data-filterable-type=substring>
              </div>
              <div class="no-results">Nothing to show</div>
            </div>
        </div>
      </div><!-- /.commitish-context-context -->
    </div>
  </div> <!-- /.scope -->

  <ul class="tabnav-tabs">
    <li><a href="/motley-git/Kernel-Nexus7" class="selected tabnav-tab" highlight="repo_source">Files</a></li>
    <li><a href="/motley-git/Kernel-Nexus7/commits/master" class="tabnav-tab" highlight="repo_commits">Commits</a></li>
    <li><a href="/motley-git/Kernel-Nexus7/branches" class="tabnav-tab" highlight="repo_branches" rel="nofollow">Branches <span class="counter ">3</span></a></li>
  </ul>

</div>

  
  
  


            
          </div>
        </div><!-- /.repohead -->

        <div id="js-repo-pjax-container" class="container context-loader-container" data-pjax-container>
          


<!-- blob contrib key: blob_contributors:v21:813878521ba48a7c2c508e0a4ed4415d -->
<!-- blob contrib frag key: views10/v8/blob_contributors:v21:813878521ba48a7c2c508e0a4ed4415d -->

<div id="slider">


    <p title="This is a placeholder element" class="js-history-link-replace hidden"></p>
    <div class="breadcrumb" data-path="drivers/staging/zram/xvmalloc.c/">
      <span class='bold'><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/motley-git/Kernel-Nexus7" class="js-slider-breadcrumb" itemscope="url"><span itemprop="title">Kernel-Nexus7</span></a></span></span> / <span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/motley-git/Kernel-Nexus7/tree/master/drivers" class="js-slider-breadcrumb" itemscope="url"><span itemprop="title">drivers</span></a></span> / <span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/motley-git/Kernel-Nexus7/tree/master/drivers/staging" class="js-slider-breadcrumb" itemscope="url"><span itemprop="title">staging</span></a></span> / <span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/motley-git/Kernel-Nexus7/tree/master/drivers/staging/zram" class="js-slider-breadcrumb" itemscope="url"><span itemprop="title">zram</span></a></span> / <strong class="final-path">xvmalloc.c</strong> <span class="js-clippy mini-icon mini-icon-clippy " data-clipboard-text="drivers/staging/zram/xvmalloc.c" data-copied-hint="copied!" data-copy-hint="copy to clipboard"></span>
    </div>

    <a href="/motley-git/Kernel-Nexus7/find/master" class="js-slide-to" data-hotkey="t" style="display:none">Show File Finder</a>

      
  <div class="commit file-history-tease" data-path="drivers/staging/zram/xvmalloc.c/">
    <img class="main-avatar" height="24" src="https://secure.gravatar.com/avatar/b2f5b4b751974a3290581998135933d1?s=140&amp;d=https://a248.e.akamai.net/assets.github.com%2Fimages%2Fgravatars%2Fgravatar-user-420.png" width="24" />
    <span class="author"><a href="/motley-git">motley-git</a></span>
    <time class="js-relative-date" datetime="2012-07-14T18:28:32-07:00" title="2012-07-14 18:28:32">July 14, 2012</time>
    <div class="commit-title">
        <a href="/motley-git/Kernel-Nexus7/commit/dad05d49ad1cc8f5b5af6a266bc1b9737b417cc8" class="message">Initial commit for Nexus 7 (Grouper)</a>
    </div>

    <div class="participation">
      <p class="quickstat"><a href="#blob_contributors_box" rel="facebox"><strong>1</strong> contributor</a></p>
      
    </div>
    <div id="blob_contributors_box" style="display:none">
      <h2>Users on GitHub who have contributed to this file</h2>
      <ul class="facebox-user-list">
        <li>
          <img height="24" src="https://secure.gravatar.com/avatar/b2f5b4b751974a3290581998135933d1?s=140&amp;d=https://a248.e.akamai.net/assets.github.com%2Fimages%2Fgravatars%2Fgravatar-user-420.png" width="24" />
          <a href="/motley-git">motley-git</a>
        </li>
      </ul>
    </div>
  </div>


    <div class="frames">
      <div class="frame frame-center" data-path="drivers/staging/zram/xvmalloc.c/" data-permalink-url="/motley-git/Kernel-Nexus7/blob/02eea732fc1f834d620eba4a5a21785fd913ab71/drivers/staging/zram/xvmalloc.c" data-title="Kernel-Nexus7/drivers/staging/zram/xvmalloc.c at master · motley-git/Kernel-Nexus7 · GitHub" data-type="blob">

        <div id="files" class="bubble">
          <div class="file">
            <div class="meta">
              <div class="info">
                <span class="icon"><b class="mini-icon mini-icon-text-file"></b></span>
                <span class="mode" title="File Mode">file</span>
                  <span>511 lines (422 sloc)</span>
                <span>12.896 kb</span>
              </div>
              <ul class="button-group actions">
                  <li>
                        <a class="grouped-button minibutton bigger lighter tooltipped leftwards"
                           title="Clicking this button will automatically fork this project so you can edit the file"
                           href="/motley-git/Kernel-Nexus7/edit/master/drivers/staging/zram/xvmalloc.c"
                           data-method="post" rel="nofollow">Edit</a>
                  </li>
                <li><a href="/motley-git/Kernel-Nexus7/raw/master/drivers/staging/zram/xvmalloc.c" class="minibutton grouped-button bigger lighter" id="raw-url">Raw</a></li>
                  <li><a href="/motley-git/Kernel-Nexus7/blame/master/drivers/staging/zram/xvmalloc.c" class="minibutton grouped-button bigger lighter">Blame</a></li>
                <li><a href="/motley-git/Kernel-Nexus7/commits/master/drivers/staging/zram/xvmalloc.c" class="minibutton grouped-button bigger lighter" rel="nofollow">History</a></li>
              </ul>
            </div>
                <div class="data type-c">
      <table cellpadding="0" cellspacing="0" class="lines">
        <tr>
          <td>
            <pre class="line_numbers"><span id="L1" rel="#L1">1</span>
<span id="L2" rel="#L2">2</span>
<span id="L3" rel="#L3">3</span>
<span id="L4" rel="#L4">4</span>
<span id="L5" rel="#L5">5</span>
<span id="L6" rel="#L6">6</span>
<span id="L7" rel="#L7">7</span>
<span id="L8" rel="#L8">8</span>
<span id="L9" rel="#L9">9</span>
<span id="L10" rel="#L10">10</span>
<span id="L11" rel="#L11">11</span>
<span id="L12" rel="#L12">12</span>
<span id="L13" rel="#L13">13</span>
<span id="L14" rel="#L14">14</span>
<span id="L15" rel="#L15">15</span>
<span id="L16" rel="#L16">16</span>
<span id="L17" rel="#L17">17</span>
<span id="L18" rel="#L18">18</span>
<span id="L19" rel="#L19">19</span>
<span id="L20" rel="#L20">20</span>
<span id="L21" rel="#L21">21</span>
<span id="L22" rel="#L22">22</span>
<span id="L23" rel="#L23">23</span>
<span id="L24" rel="#L24">24</span>
<span id="L25" rel="#L25">25</span>
<span id="L26" rel="#L26">26</span>
<span id="L27" rel="#L27">27</span>
<span id="L28" rel="#L28">28</span>
<span id="L29" rel="#L29">29</span>
<span id="L30" rel="#L30">30</span>
<span id="L31" rel="#L31">31</span>
<span id="L32" rel="#L32">32</span>
<span id="L33" rel="#L33">33</span>
<span id="L34" rel="#L34">34</span>
<span id="L35" rel="#L35">35</span>
<span id="L36" rel="#L36">36</span>
<span id="L37" rel="#L37">37</span>
<span id="L38" rel="#L38">38</span>
<span id="L39" rel="#L39">39</span>
<span id="L40" rel="#L40">40</span>
<span id="L41" rel="#L41">41</span>
<span id="L42" rel="#L42">42</span>
<span id="L43" rel="#L43">43</span>
<span id="L44" rel="#L44">44</span>
<span id="L45" rel="#L45">45</span>
<span id="L46" rel="#L46">46</span>
<span id="L47" rel="#L47">47</span>
<span id="L48" rel="#L48">48</span>
<span id="L49" rel="#L49">49</span>
<span id="L50" rel="#L50">50</span>
<span id="L51" rel="#L51">51</span>
<span id="L52" rel="#L52">52</span>
<span id="L53" rel="#L53">53</span>
<span id="L54" rel="#L54">54</span>
<span id="L55" rel="#L55">55</span>
<span id="L56" rel="#L56">56</span>
<span id="L57" rel="#L57">57</span>
<span id="L58" rel="#L58">58</span>
<span id="L59" rel="#L59">59</span>
<span id="L60" rel="#L60">60</span>
<span id="L61" rel="#L61">61</span>
<span id="L62" rel="#L62">62</span>
<span id="L63" rel="#L63">63</span>
<span id="L64" rel="#L64">64</span>
<span id="L65" rel="#L65">65</span>
<span id="L66" rel="#L66">66</span>
<span id="L67" rel="#L67">67</span>
<span id="L68" rel="#L68">68</span>
<span id="L69" rel="#L69">69</span>
<span id="L70" rel="#L70">70</span>
<span id="L71" rel="#L71">71</span>
<span id="L72" rel="#L72">72</span>
<span id="L73" rel="#L73">73</span>
<span id="L74" rel="#L74">74</span>
<span id="L75" rel="#L75">75</span>
<span id="L76" rel="#L76">76</span>
<span id="L77" rel="#L77">77</span>
<span id="L78" rel="#L78">78</span>
<span id="L79" rel="#L79">79</span>
<span id="L80" rel="#L80">80</span>
<span id="L81" rel="#L81">81</span>
<span id="L82" rel="#L82">82</span>
<span id="L83" rel="#L83">83</span>
<span id="L84" rel="#L84">84</span>
<span id="L85" rel="#L85">85</span>
<span id="L86" rel="#L86">86</span>
<span id="L87" rel="#L87">87</span>
<span id="L88" rel="#L88">88</span>
<span id="L89" rel="#L89">89</span>
<span id="L90" rel="#L90">90</span>
<span id="L91" rel="#L91">91</span>
<span id="L92" rel="#L92">92</span>
<span id="L93" rel="#L93">93</span>
<span id="L94" rel="#L94">94</span>
<span id="L95" rel="#L95">95</span>
<span id="L96" rel="#L96">96</span>
<span id="L97" rel="#L97">97</span>
<span id="L98" rel="#L98">98</span>
<span id="L99" rel="#L99">99</span>
<span id="L100" rel="#L100">100</span>
<span id="L101" rel="#L101">101</span>
<span id="L102" rel="#L102">102</span>
<span id="L103" rel="#L103">103</span>
<span id="L104" rel="#L104">104</span>
<span id="L105" rel="#L105">105</span>
<span id="L106" rel="#L106">106</span>
<span id="L107" rel="#L107">107</span>
<span id="L108" rel="#L108">108</span>
<span id="L109" rel="#L109">109</span>
<span id="L110" rel="#L110">110</span>
<span id="L111" rel="#L111">111</span>
<span id="L112" rel="#L112">112</span>
<span id="L113" rel="#L113">113</span>
<span id="L114" rel="#L114">114</span>
<span id="L115" rel="#L115">115</span>
<span id="L116" rel="#L116">116</span>
<span id="L117" rel="#L117">117</span>
<span id="L118" rel="#L118">118</span>
<span id="L119" rel="#L119">119</span>
<span id="L120" rel="#L120">120</span>
<span id="L121" rel="#L121">121</span>
<span id="L122" rel="#L122">122</span>
<span id="L123" rel="#L123">123</span>
<span id="L124" rel="#L124">124</span>
<span id="L125" rel="#L125">125</span>
<span id="L126" rel="#L126">126</span>
<span id="L127" rel="#L127">127</span>
<span id="L128" rel="#L128">128</span>
<span id="L129" rel="#L129">129</span>
<span id="L130" rel="#L130">130</span>
<span id="L131" rel="#L131">131</span>
<span id="L132" rel="#L132">132</span>
<span id="L133" rel="#L133">133</span>
<span id="L134" rel="#L134">134</span>
<span id="L135" rel="#L135">135</span>
<span id="L136" rel="#L136">136</span>
<span id="L137" rel="#L137">137</span>
<span id="L138" rel="#L138">138</span>
<span id="L139" rel="#L139">139</span>
<span id="L140" rel="#L140">140</span>
<span id="L141" rel="#L141">141</span>
<span id="L142" rel="#L142">142</span>
<span id="L143" rel="#L143">143</span>
<span id="L144" rel="#L144">144</span>
<span id="L145" rel="#L145">145</span>
<span id="L146" rel="#L146">146</span>
<span id="L147" rel="#L147">147</span>
<span id="L148" rel="#L148">148</span>
<span id="L149" rel="#L149">149</span>
<span id="L150" rel="#L150">150</span>
<span id="L151" rel="#L151">151</span>
<span id="L152" rel="#L152">152</span>
<span id="L153" rel="#L153">153</span>
<span id="L154" rel="#L154">154</span>
<span id="L155" rel="#L155">155</span>
<span id="L156" rel="#L156">156</span>
<span id="L157" rel="#L157">157</span>
<span id="L158" rel="#L158">158</span>
<span id="L159" rel="#L159">159</span>
<span id="L160" rel="#L160">160</span>
<span id="L161" rel="#L161">161</span>
<span id="L162" rel="#L162">162</span>
<span id="L163" rel="#L163">163</span>
<span id="L164" rel="#L164">164</span>
<span id="L165" rel="#L165">165</span>
<span id="L166" rel="#L166">166</span>
<span id="L167" rel="#L167">167</span>
<span id="L168" rel="#L168">168</span>
<span id="L169" rel="#L169">169</span>
<span id="L170" rel="#L170">170</span>
<span id="L171" rel="#L171">171</span>
<span id="L172" rel="#L172">172</span>
<span id="L173" rel="#L173">173</span>
<span id="L174" rel="#L174">174</span>
<span id="L175" rel="#L175">175</span>
<span id="L176" rel="#L176">176</span>
<span id="L177" rel="#L177">177</span>
<span id="L178" rel="#L178">178</span>
<span id="L179" rel="#L179">179</span>
<span id="L180" rel="#L180">180</span>
<span id="L181" rel="#L181">181</span>
<span id="L182" rel="#L182">182</span>
<span id="L183" rel="#L183">183</span>
<span id="L184" rel="#L184">184</span>
<span id="L185" rel="#L185">185</span>
<span id="L186" rel="#L186">186</span>
<span id="L187" rel="#L187">187</span>
<span id="L188" rel="#L188">188</span>
<span id="L189" rel="#L189">189</span>
<span id="L190" rel="#L190">190</span>
<span id="L191" rel="#L191">191</span>
<span id="L192" rel="#L192">192</span>
<span id="L193" rel="#L193">193</span>
<span id="L194" rel="#L194">194</span>
<span id="L195" rel="#L195">195</span>
<span id="L196" rel="#L196">196</span>
<span id="L197" rel="#L197">197</span>
<span id="L198" rel="#L198">198</span>
<span id="L199" rel="#L199">199</span>
<span id="L200" rel="#L200">200</span>
<span id="L201" rel="#L201">201</span>
<span id="L202" rel="#L202">202</span>
<span id="L203" rel="#L203">203</span>
<span id="L204" rel="#L204">204</span>
<span id="L205" rel="#L205">205</span>
<span id="L206" rel="#L206">206</span>
<span id="L207" rel="#L207">207</span>
<span id="L208" rel="#L208">208</span>
<span id="L209" rel="#L209">209</span>
<span id="L210" rel="#L210">210</span>
<span id="L211" rel="#L211">211</span>
<span id="L212" rel="#L212">212</span>
<span id="L213" rel="#L213">213</span>
<span id="L214" rel="#L214">214</span>
<span id="L215" rel="#L215">215</span>
<span id="L216" rel="#L216">216</span>
<span id="L217" rel="#L217">217</span>
<span id="L218" rel="#L218">218</span>
<span id="L219" rel="#L219">219</span>
<span id="L220" rel="#L220">220</span>
<span id="L221" rel="#L221">221</span>
<span id="L222" rel="#L222">222</span>
<span id="L223" rel="#L223">223</span>
<span id="L224" rel="#L224">224</span>
<span id="L225" rel="#L225">225</span>
<span id="L226" rel="#L226">226</span>
<span id="L227" rel="#L227">227</span>
<span id="L228" rel="#L228">228</span>
<span id="L229" rel="#L229">229</span>
<span id="L230" rel="#L230">230</span>
<span id="L231" rel="#L231">231</span>
<span id="L232" rel="#L232">232</span>
<span id="L233" rel="#L233">233</span>
<span id="L234" rel="#L234">234</span>
<span id="L235" rel="#L235">235</span>
<span id="L236" rel="#L236">236</span>
<span id="L237" rel="#L237">237</span>
<span id="L238" rel="#L238">238</span>
<span id="L239" rel="#L239">239</span>
<span id="L240" rel="#L240">240</span>
<span id="L241" rel="#L241">241</span>
<span id="L242" rel="#L242">242</span>
<span id="L243" rel="#L243">243</span>
<span id="L244" rel="#L244">244</span>
<span id="L245" rel="#L245">245</span>
<span id="L246" rel="#L246">246</span>
<span id="L247" rel="#L247">247</span>
<span id="L248" rel="#L248">248</span>
<span id="L249" rel="#L249">249</span>
<span id="L250" rel="#L250">250</span>
<span id="L251" rel="#L251">251</span>
<span id="L252" rel="#L252">252</span>
<span id="L253" rel="#L253">253</span>
<span id="L254" rel="#L254">254</span>
<span id="L255" rel="#L255">255</span>
<span id="L256" rel="#L256">256</span>
<span id="L257" rel="#L257">257</span>
<span id="L258" rel="#L258">258</span>
<span id="L259" rel="#L259">259</span>
<span id="L260" rel="#L260">260</span>
<span id="L261" rel="#L261">261</span>
<span id="L262" rel="#L262">262</span>
<span id="L263" rel="#L263">263</span>
<span id="L264" rel="#L264">264</span>
<span id="L265" rel="#L265">265</span>
<span id="L266" rel="#L266">266</span>
<span id="L267" rel="#L267">267</span>
<span id="L268" rel="#L268">268</span>
<span id="L269" rel="#L269">269</span>
<span id="L270" rel="#L270">270</span>
<span id="L271" rel="#L271">271</span>
<span id="L272" rel="#L272">272</span>
<span id="L273" rel="#L273">273</span>
<span id="L274" rel="#L274">274</span>
<span id="L275" rel="#L275">275</span>
<span id="L276" rel="#L276">276</span>
<span id="L277" rel="#L277">277</span>
<span id="L278" rel="#L278">278</span>
<span id="L279" rel="#L279">279</span>
<span id="L280" rel="#L280">280</span>
<span id="L281" rel="#L281">281</span>
<span id="L282" rel="#L282">282</span>
<span id="L283" rel="#L283">283</span>
<span id="L284" rel="#L284">284</span>
<span id="L285" rel="#L285">285</span>
<span id="L286" rel="#L286">286</span>
<span id="L287" rel="#L287">287</span>
<span id="L288" rel="#L288">288</span>
<span id="L289" rel="#L289">289</span>
<span id="L290" rel="#L290">290</span>
<span id="L291" rel="#L291">291</span>
<span id="L292" rel="#L292">292</span>
<span id="L293" rel="#L293">293</span>
<span id="L294" rel="#L294">294</span>
<span id="L295" rel="#L295">295</span>
<span id="L296" rel="#L296">296</span>
<span id="L297" rel="#L297">297</span>
<span id="L298" rel="#L298">298</span>
<span id="L299" rel="#L299">299</span>
<span id="L300" rel="#L300">300</span>
<span id="L301" rel="#L301">301</span>
<span id="L302" rel="#L302">302</span>
<span id="L303" rel="#L303">303</span>
<span id="L304" rel="#L304">304</span>
<span id="L305" rel="#L305">305</span>
<span id="L306" rel="#L306">306</span>
<span id="L307" rel="#L307">307</span>
<span id="L308" rel="#L308">308</span>
<span id="L309" rel="#L309">309</span>
<span id="L310" rel="#L310">310</span>
<span id="L311" rel="#L311">311</span>
<span id="L312" rel="#L312">312</span>
<span id="L313" rel="#L313">313</span>
<span id="L314" rel="#L314">314</span>
<span id="L315" rel="#L315">315</span>
<span id="L316" rel="#L316">316</span>
<span id="L317" rel="#L317">317</span>
<span id="L318" rel="#L318">318</span>
<span id="L319" rel="#L319">319</span>
<span id="L320" rel="#L320">320</span>
<span id="L321" rel="#L321">321</span>
<span id="L322" rel="#L322">322</span>
<span id="L323" rel="#L323">323</span>
<span id="L324" rel="#L324">324</span>
<span id="L325" rel="#L325">325</span>
<span id="L326" rel="#L326">326</span>
<span id="L327" rel="#L327">327</span>
<span id="L328" rel="#L328">328</span>
<span id="L329" rel="#L329">329</span>
<span id="L330" rel="#L330">330</span>
<span id="L331" rel="#L331">331</span>
<span id="L332" rel="#L332">332</span>
<span id="L333" rel="#L333">333</span>
<span id="L334" rel="#L334">334</span>
<span id="L335" rel="#L335">335</span>
<span id="L336" rel="#L336">336</span>
<span id="L337" rel="#L337">337</span>
<span id="L338" rel="#L338">338</span>
<span id="L339" rel="#L339">339</span>
<span id="L340" rel="#L340">340</span>
<span id="L341" rel="#L341">341</span>
<span id="L342" rel="#L342">342</span>
<span id="L343" rel="#L343">343</span>
<span id="L344" rel="#L344">344</span>
<span id="L345" rel="#L345">345</span>
<span id="L346" rel="#L346">346</span>
<span id="L347" rel="#L347">347</span>
<span id="L348" rel="#L348">348</span>
<span id="L349" rel="#L349">349</span>
<span id="L350" rel="#L350">350</span>
<span id="L351" rel="#L351">351</span>
<span id="L352" rel="#L352">352</span>
<span id="L353" rel="#L353">353</span>
<span id="L354" rel="#L354">354</span>
<span id="L355" rel="#L355">355</span>
<span id="L356" rel="#L356">356</span>
<span id="L357" rel="#L357">357</span>
<span id="L358" rel="#L358">358</span>
<span id="L359" rel="#L359">359</span>
<span id="L360" rel="#L360">360</span>
<span id="L361" rel="#L361">361</span>
<span id="L362" rel="#L362">362</span>
<span id="L363" rel="#L363">363</span>
<span id="L364" rel="#L364">364</span>
<span id="L365" rel="#L365">365</span>
<span id="L366" rel="#L366">366</span>
<span id="L367" rel="#L367">367</span>
<span id="L368" rel="#L368">368</span>
<span id="L369" rel="#L369">369</span>
<span id="L370" rel="#L370">370</span>
<span id="L371" rel="#L371">371</span>
<span id="L372" rel="#L372">372</span>
<span id="L373" rel="#L373">373</span>
<span id="L374" rel="#L374">374</span>
<span id="L375" rel="#L375">375</span>
<span id="L376" rel="#L376">376</span>
<span id="L377" rel="#L377">377</span>
<span id="L378" rel="#L378">378</span>
<span id="L379" rel="#L379">379</span>
<span id="L380" rel="#L380">380</span>
<span id="L381" rel="#L381">381</span>
<span id="L382" rel="#L382">382</span>
<span id="L383" rel="#L383">383</span>
<span id="L384" rel="#L384">384</span>
<span id="L385" rel="#L385">385</span>
<span id="L386" rel="#L386">386</span>
<span id="L387" rel="#L387">387</span>
<span id="L388" rel="#L388">388</span>
<span id="L389" rel="#L389">389</span>
<span id="L390" rel="#L390">390</span>
<span id="L391" rel="#L391">391</span>
<span id="L392" rel="#L392">392</span>
<span id="L393" rel="#L393">393</span>
<span id="L394" rel="#L394">394</span>
<span id="L395" rel="#L395">395</span>
<span id="L396" rel="#L396">396</span>
<span id="L397" rel="#L397">397</span>
<span id="L398" rel="#L398">398</span>
<span id="L399" rel="#L399">399</span>
<span id="L400" rel="#L400">400</span>
<span id="L401" rel="#L401">401</span>
<span id="L402" rel="#L402">402</span>
<span id="L403" rel="#L403">403</span>
<span id="L404" rel="#L404">404</span>
<span id="L405" rel="#L405">405</span>
<span id="L406" rel="#L406">406</span>
<span id="L407" rel="#L407">407</span>
<span id="L408" rel="#L408">408</span>
<span id="L409" rel="#L409">409</span>
<span id="L410" rel="#L410">410</span>
<span id="L411" rel="#L411">411</span>
<span id="L412" rel="#L412">412</span>
<span id="L413" rel="#L413">413</span>
<span id="L414" rel="#L414">414</span>
<span id="L415" rel="#L415">415</span>
<span id="L416" rel="#L416">416</span>
<span id="L417" rel="#L417">417</span>
<span id="L418" rel="#L418">418</span>
<span id="L419" rel="#L419">419</span>
<span id="L420" rel="#L420">420</span>
<span id="L421" rel="#L421">421</span>
<span id="L422" rel="#L422">422</span>
<span id="L423" rel="#L423">423</span>
<span id="L424" rel="#L424">424</span>
<span id="L425" rel="#L425">425</span>
<span id="L426" rel="#L426">426</span>
<span id="L427" rel="#L427">427</span>
<span id="L428" rel="#L428">428</span>
<span id="L429" rel="#L429">429</span>
<span id="L430" rel="#L430">430</span>
<span id="L431" rel="#L431">431</span>
<span id="L432" rel="#L432">432</span>
<span id="L433" rel="#L433">433</span>
<span id="L434" rel="#L434">434</span>
<span id="L435" rel="#L435">435</span>
<span id="L436" rel="#L436">436</span>
<span id="L437" rel="#L437">437</span>
<span id="L438" rel="#L438">438</span>
<span id="L439" rel="#L439">439</span>
<span id="L440" rel="#L440">440</span>
<span id="L441" rel="#L441">441</span>
<span id="L442" rel="#L442">442</span>
<span id="L443" rel="#L443">443</span>
<span id="L444" rel="#L444">444</span>
<span id="L445" rel="#L445">445</span>
<span id="L446" rel="#L446">446</span>
<span id="L447" rel="#L447">447</span>
<span id="L448" rel="#L448">448</span>
<span id="L449" rel="#L449">449</span>
<span id="L450" rel="#L450">450</span>
<span id="L451" rel="#L451">451</span>
<span id="L452" rel="#L452">452</span>
<span id="L453" rel="#L453">453</span>
<span id="L454" rel="#L454">454</span>
<span id="L455" rel="#L455">455</span>
<span id="L456" rel="#L456">456</span>
<span id="L457" rel="#L457">457</span>
<span id="L458" rel="#L458">458</span>
<span id="L459" rel="#L459">459</span>
<span id="L460" rel="#L460">460</span>
<span id="L461" rel="#L461">461</span>
<span id="L462" rel="#L462">462</span>
<span id="L463" rel="#L463">463</span>
<span id="L464" rel="#L464">464</span>
<span id="L465" rel="#L465">465</span>
<span id="L466" rel="#L466">466</span>
<span id="L467" rel="#L467">467</span>
<span id="L468" rel="#L468">468</span>
<span id="L469" rel="#L469">469</span>
<span id="L470" rel="#L470">470</span>
<span id="L471" rel="#L471">471</span>
<span id="L472" rel="#L472">472</span>
<span id="L473" rel="#L473">473</span>
<span id="L474" rel="#L474">474</span>
<span id="L475" rel="#L475">475</span>
<span id="L476" rel="#L476">476</span>
<span id="L477" rel="#L477">477</span>
<span id="L478" rel="#L478">478</span>
<span id="L479" rel="#L479">479</span>
<span id="L480" rel="#L480">480</span>
<span id="L481" rel="#L481">481</span>
<span id="L482" rel="#L482">482</span>
<span id="L483" rel="#L483">483</span>
<span id="L484" rel="#L484">484</span>
<span id="L485" rel="#L485">485</span>
<span id="L486" rel="#L486">486</span>
<span id="L487" rel="#L487">487</span>
<span id="L488" rel="#L488">488</span>
<span id="L489" rel="#L489">489</span>
<span id="L490" rel="#L490">490</span>
<span id="L491" rel="#L491">491</span>
<span id="L492" rel="#L492">492</span>
<span id="L493" rel="#L493">493</span>
<span id="L494" rel="#L494">494</span>
<span id="L495" rel="#L495">495</span>
<span id="L496" rel="#L496">496</span>
<span id="L497" rel="#L497">497</span>
<span id="L498" rel="#L498">498</span>
<span id="L499" rel="#L499">499</span>
<span id="L500" rel="#L500">500</span>
<span id="L501" rel="#L501">501</span>
<span id="L502" rel="#L502">502</span>
<span id="L503" rel="#L503">503</span>
<span id="L504" rel="#L504">504</span>
<span id="L505" rel="#L505">505</span>
<span id="L506" rel="#L506">506</span>
<span id="L507" rel="#L507">507</span>
<span id="L508" rel="#L508">508</span>
<span id="L509" rel="#L509">509</span>
<span id="L510" rel="#L510">510</span>
</pre>
          </td>
          <td width="100%">
                <div class="highlight"><pre><div class='line' id='LC1'><span class="cm">/*</span></div><div class='line' id='LC2'><span class="cm"> * xvmalloc memory allocator</span></div><div class='line' id='LC3'><span class="cm"> *</span></div><div class='line' id='LC4'><span class="cm"> * Copyright (C) 2008, 2009, 2010  Nitin Gupta</span></div><div class='line' id='LC5'><span class="cm"> *</span></div><div class='line' id='LC6'><span class="cm"> * This code is released using a dual license strategy: BSD/GPL</span></div><div class='line' id='LC7'><span class="cm"> * You can choose the licence that better fits your requirements.</span></div><div class='line' id='LC8'><span class="cm"> *</span></div><div class='line' id='LC9'><span class="cm"> * Released under the terms of 3-clause BSD License</span></div><div class='line' id='LC10'><span class="cm"> * Released under the terms of GNU General Public License Version 2.0</span></div><div class='line' id='LC11'><span class="cm"> */</span></div><div class='line' id='LC12'><br/></div><div class='line' id='LC13'><span class="cp">#ifdef CONFIG_ZRAM_DEBUG</span></div><div class='line' id='LC14'><span class="cp">#define DEBUG</span></div><div class='line' id='LC15'><span class="cp">#endif</span></div><div class='line' id='LC16'><br/></div><div class='line' id='LC17'><span class="cp">#include &lt;linux/module.h&gt;</span></div><div class='line' id='LC18'><span class="cp">#include &lt;linux/kernel.h&gt;</span></div><div class='line' id='LC19'><span class="cp">#include &lt;linux/bitops.h&gt;</span></div><div class='line' id='LC20'><span class="cp">#include &lt;linux/errno.h&gt;</span></div><div class='line' id='LC21'><span class="cp">#include &lt;linux/highmem.h&gt;</span></div><div class='line' id='LC22'><span class="cp">#include &lt;linux/init.h&gt;</span></div><div class='line' id='LC23'><span class="cp">#include &lt;linux/string.h&gt;</span></div><div class='line' id='LC24'><span class="cp">#include &lt;linux/slab.h&gt;</span></div><div class='line' id='LC25'><br/></div><div class='line' id='LC26'><span class="cp">#include &quot;xvmalloc.h&quot;</span></div><div class='line' id='LC27'><span class="cp">#include &quot;xvmalloc_int.h&quot;</span></div><div class='line' id='LC28'><br/></div><div class='line' id='LC29'><span class="k">static</span> <span class="kt">void</span> <span class="nf">stat_inc</span><span class="p">(</span><span class="n">u64</span> <span class="o">*</span><span class="n">value</span><span class="p">)</span></div><div class='line' id='LC30'><span class="p">{</span></div><div class='line' id='LC31'>	<span class="o">*</span><span class="n">value</span> <span class="o">=</span> <span class="o">*</span><span class="n">value</span> <span class="o">+</span> <span class="mi">1</span><span class="p">;</span></div><div class='line' id='LC32'><span class="p">}</span></div><div class='line' id='LC33'><br/></div><div class='line' id='LC34'><span class="k">static</span> <span class="kt">void</span> <span class="nf">stat_dec</span><span class="p">(</span><span class="n">u64</span> <span class="o">*</span><span class="n">value</span><span class="p">)</span></div><div class='line' id='LC35'><span class="p">{</span></div><div class='line' id='LC36'>	<span class="o">*</span><span class="n">value</span> <span class="o">=</span> <span class="o">*</span><span class="n">value</span> <span class="o">-</span> <span class="mi">1</span><span class="p">;</span></div><div class='line' id='LC37'><span class="p">}</span></div><div class='line' id='LC38'><br/></div><div class='line' id='LC39'><span class="k">static</span> <span class="kt">int</span> <span class="nf">test_flag</span><span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">,</span> <span class="k">enum</span> <span class="n">blockflags</span> <span class="n">flag</span><span class="p">)</span></div><div class='line' id='LC40'><span class="p">{</span></div><div class='line' id='LC41'>	<span class="k">return</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">prev</span> <span class="o">&amp;</span> <span class="n">BIT</span><span class="p">(</span><span class="n">flag</span><span class="p">);</span></div><div class='line' id='LC42'><span class="p">}</span></div><div class='line' id='LC43'><br/></div><div class='line' id='LC44'><span class="k">static</span> <span class="kt">void</span> <span class="nf">set_flag</span><span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">,</span> <span class="k">enum</span> <span class="n">blockflags</span> <span class="n">flag</span><span class="p">)</span></div><div class='line' id='LC45'><span class="p">{</span></div><div class='line' id='LC46'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">prev</span> <span class="o">|=</span> <span class="n">BIT</span><span class="p">(</span><span class="n">flag</span><span class="p">);</span></div><div class='line' id='LC47'><span class="p">}</span></div><div class='line' id='LC48'><br/></div><div class='line' id='LC49'><span class="k">static</span> <span class="kt">void</span> <span class="nf">clear_flag</span><span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">,</span> <span class="k">enum</span> <span class="n">blockflags</span> <span class="n">flag</span><span class="p">)</span></div><div class='line' id='LC50'><span class="p">{</span></div><div class='line' id='LC51'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">prev</span> <span class="o">&amp;=</span> <span class="o">~</span><span class="n">BIT</span><span class="p">(</span><span class="n">flag</span><span class="p">);</span></div><div class='line' id='LC52'><span class="p">}</span></div><div class='line' id='LC53'><br/></div><div class='line' id='LC54'><span class="cm">/*</span></div><div class='line' id='LC55'><span class="cm"> * Given &lt;page, offset&gt; pair, provide a dereferencable pointer.</span></div><div class='line' id='LC56'><span class="cm"> * This is called from xv_malloc/xv_free path, so it</span></div><div class='line' id='LC57'><span class="cm"> * needs to be fast.</span></div><div class='line' id='LC58'><span class="cm"> */</span></div><div class='line' id='LC59'><span class="k">static</span> <span class="kt">void</span> <span class="o">*</span><span class="nf">get_ptr_atomic</span><span class="p">(</span><span class="k">struct</span> <span class="n">page</span> <span class="o">*</span><span class="n">page</span><span class="p">,</span> <span class="n">u16</span> <span class="n">offset</span><span class="p">,</span> <span class="k">enum</span> <span class="n">km_type</span> <span class="n">type</span><span class="p">)</span></div><div class='line' id='LC60'><span class="p">{</span></div><div class='line' id='LC61'>	<span class="kt">unsigned</span> <span class="kt">char</span> <span class="o">*</span><span class="n">base</span><span class="p">;</span></div><div class='line' id='LC62'><br/></div><div class='line' id='LC63'>	<span class="n">base</span> <span class="o">=</span> <span class="n">kmap_atomic</span><span class="p">(</span><span class="n">page</span><span class="p">,</span> <span class="n">type</span><span class="p">);</span></div><div class='line' id='LC64'>	<span class="k">return</span> <span class="n">base</span> <span class="o">+</span> <span class="n">offset</span><span class="p">;</span></div><div class='line' id='LC65'><span class="p">}</span></div><div class='line' id='LC66'><br/></div><div class='line' id='LC67'><span class="k">static</span> <span class="kt">void</span> <span class="nf">put_ptr_atomic</span><span class="p">(</span><span class="kt">void</span> <span class="o">*</span><span class="n">ptr</span><span class="p">,</span> <span class="k">enum</span> <span class="n">km_type</span> <span class="n">type</span><span class="p">)</span></div><div class='line' id='LC68'><span class="p">{</span></div><div class='line' id='LC69'>	<span class="n">kunmap_atomic</span><span class="p">(</span><span class="n">ptr</span><span class="p">,</span> <span class="n">type</span><span class="p">);</span></div><div class='line' id='LC70'><span class="p">}</span></div><div class='line' id='LC71'><br/></div><div class='line' id='LC72'><span class="k">static</span> <span class="n">u32</span> <span class="nf">get_blockprev</span><span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">)</span></div><div class='line' id='LC73'><span class="p">{</span></div><div class='line' id='LC74'>	<span class="k">return</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">prev</span> <span class="o">&amp;</span> <span class="n">PREV_MASK</span><span class="p">;</span></div><div class='line' id='LC75'><span class="p">}</span></div><div class='line' id='LC76'><br/></div><div class='line' id='LC77'><span class="k">static</span> <span class="kt">void</span> <span class="nf">set_blockprev</span><span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">,</span> <span class="n">u16</span> <span class="n">new_offset</span><span class="p">)</span></div><div class='line' id='LC78'><span class="p">{</span></div><div class='line' id='LC79'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">prev</span> <span class="o">=</span> <span class="n">new_offset</span> <span class="o">|</span> <span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">prev</span> <span class="o">&amp;</span> <span class="n">FLAGS_MASK</span><span class="p">);</span></div><div class='line' id='LC80'><span class="p">}</span></div><div class='line' id='LC81'><br/></div><div class='line' id='LC82'><span class="k">static</span> <span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="nf">BLOCK_NEXT</span><span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">)</span></div><div class='line' id='LC83'><span class="p">{</span></div><div class='line' id='LC84'>	<span class="k">return</span> <span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="p">)</span></div><div class='line' id='LC85'>		<span class="p">((</span><span class="kt">char</span> <span class="o">*</span><span class="p">)</span><span class="n">block</span> <span class="o">+</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">+</span> <span class="n">XV_ALIGN</span><span class="p">);</span></div><div class='line' id='LC86'><span class="p">}</span></div><div class='line' id='LC87'><br/></div><div class='line' id='LC88'><span class="cm">/*</span></div><div class='line' id='LC89'><span class="cm"> * Get index of free list containing blocks of maximum size</span></div><div class='line' id='LC90'><span class="cm"> * which is less than or equal to given size.</span></div><div class='line' id='LC91'><span class="cm"> */</span></div><div class='line' id='LC92'><span class="k">static</span> <span class="n">u32</span> <span class="nf">get_index_for_insert</span><span class="p">(</span><span class="n">u32</span> <span class="n">size</span><span class="p">)</span></div><div class='line' id='LC93'><span class="p">{</span></div><div class='line' id='LC94'>	<span class="k">if</span> <span class="p">(</span><span class="n">unlikely</span><span class="p">(</span><span class="n">size</span> <span class="o">&gt;</span> <span class="n">XV_MAX_ALLOC_SIZE</span><span class="p">))</span></div><div class='line' id='LC95'>		<span class="n">size</span> <span class="o">=</span> <span class="n">XV_MAX_ALLOC_SIZE</span><span class="p">;</span></div><div class='line' id='LC96'>	<span class="n">size</span> <span class="o">&amp;=</span> <span class="o">~</span><span class="n">FL_DELTA_MASK</span><span class="p">;</span></div><div class='line' id='LC97'>	<span class="k">return</span> <span class="p">(</span><span class="n">size</span> <span class="o">-</span> <span class="n">XV_MIN_ALLOC_SIZE</span><span class="p">)</span> <span class="o">&gt;&gt;</span> <span class="n">FL_DELTA_SHIFT</span><span class="p">;</span></div><div class='line' id='LC98'><span class="p">}</span></div><div class='line' id='LC99'><br/></div><div class='line' id='LC100'><span class="cm">/*</span></div><div class='line' id='LC101'><span class="cm"> * Get index of free list having blocks of size greater than</span></div><div class='line' id='LC102'><span class="cm"> * or equal to requested size.</span></div><div class='line' id='LC103'><span class="cm"> */</span></div><div class='line' id='LC104'><span class="k">static</span> <span class="n">u32</span> <span class="nf">get_index</span><span class="p">(</span><span class="n">u32</span> <span class="n">size</span><span class="p">)</span></div><div class='line' id='LC105'><span class="p">{</span></div><div class='line' id='LC106'>	<span class="k">if</span> <span class="p">(</span><span class="n">unlikely</span><span class="p">(</span><span class="n">size</span> <span class="o">&lt;</span> <span class="n">XV_MIN_ALLOC_SIZE</span><span class="p">))</span></div><div class='line' id='LC107'>		<span class="n">size</span> <span class="o">=</span> <span class="n">XV_MIN_ALLOC_SIZE</span><span class="p">;</span></div><div class='line' id='LC108'>	<span class="n">size</span> <span class="o">=</span> <span class="n">ALIGN</span><span class="p">(</span><span class="n">size</span><span class="p">,</span> <span class="n">FL_DELTA</span><span class="p">);</span></div><div class='line' id='LC109'>	<span class="k">return</span> <span class="p">(</span><span class="n">size</span> <span class="o">-</span> <span class="n">XV_MIN_ALLOC_SIZE</span><span class="p">)</span> <span class="o">&gt;&gt;</span> <span class="n">FL_DELTA_SHIFT</span><span class="p">;</span></div><div class='line' id='LC110'><span class="p">}</span></div><div class='line' id='LC111'><br/></div><div class='line' id='LC112'><span class="cm">/**</span></div><div class='line' id='LC113'><span class="cm"> * find_block - find block of at least given size</span></div><div class='line' id='LC114'><span class="cm"> * @pool: memory pool to search from</span></div><div class='line' id='LC115'><span class="cm"> * @size: size of block required</span></div><div class='line' id='LC116'><span class="cm"> * @page: page containing required block</span></div><div class='line' id='LC117'><span class="cm"> * @offset: offset within the page where block is located.</span></div><div class='line' id='LC118'><span class="cm"> *</span></div><div class='line' id='LC119'><span class="cm"> * Searches two level bitmap to locate block of at least</span></div><div class='line' id='LC120'><span class="cm"> * the given size. If such a block is found, it provides</span></div><div class='line' id='LC121'><span class="cm"> * &lt;page, offset&gt; to identify this block and returns index</span></div><div class='line' id='LC122'><span class="cm"> * in freelist where we found this block.</span></div><div class='line' id='LC123'><span class="cm"> * Otherwise, returns 0 and &lt;page, offset&gt; params are not touched.</span></div><div class='line' id='LC124'><span class="cm"> */</span></div><div class='line' id='LC125'><span class="k">static</span> <span class="n">u32</span> <span class="nf">find_block</span><span class="p">(</span><span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="n">pool</span><span class="p">,</span> <span class="n">u32</span> <span class="n">size</span><span class="p">,</span></div><div class='line' id='LC126'>			<span class="k">struct</span> <span class="n">page</span> <span class="o">**</span><span class="n">page</span><span class="p">,</span> <span class="n">u32</span> <span class="o">*</span><span class="n">offset</span><span class="p">)</span></div><div class='line' id='LC127'><span class="p">{</span></div><div class='line' id='LC128'>	<span class="n">ulong</span> <span class="n">flbitmap</span><span class="p">,</span> <span class="n">slbitmap</span><span class="p">;</span></div><div class='line' id='LC129'>	<span class="n">u32</span> <span class="n">flindex</span><span class="p">,</span> <span class="n">slindex</span><span class="p">,</span> <span class="n">slbitstart</span><span class="p">;</span></div><div class='line' id='LC130'><br/></div><div class='line' id='LC131'>	<span class="cm">/* There are no free blocks in this pool */</span></div><div class='line' id='LC132'>	<span class="k">if</span> <span class="p">(</span><span class="o">!</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">flbitmap</span><span class="p">)</span></div><div class='line' id='LC133'>		<span class="k">return</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC134'><br/></div><div class='line' id='LC135'>	<span class="cm">/* Get freelist index correspoding to this size */</span></div><div class='line' id='LC136'>	<span class="n">slindex</span> <span class="o">=</span> <span class="n">get_index</span><span class="p">(</span><span class="n">size</span><span class="p">);</span></div><div class='line' id='LC137'>	<span class="n">slbitmap</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">slbitmap</span><span class="p">[</span><span class="n">slindex</span> <span class="o">/</span> <span class="n">BITS_PER_LONG</span><span class="p">];</span></div><div class='line' id='LC138'>	<span class="n">slbitstart</span> <span class="o">=</span> <span class="n">slindex</span> <span class="o">%</span> <span class="n">BITS_PER_LONG</span><span class="p">;</span></div><div class='line' id='LC139'><br/></div><div class='line' id='LC140'>	<span class="cm">/*</span></div><div class='line' id='LC141'><span class="cm">	 * If freelist is not empty at this index, we found the</span></div><div class='line' id='LC142'><span class="cm">	 * block - head of this list. This is approximate best-fit match.</span></div><div class='line' id='LC143'><span class="cm">	 */</span></div><div class='line' id='LC144'>	<span class="k">if</span> <span class="p">(</span><span class="n">test_bit</span><span class="p">(</span><span class="n">slbitstart</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">slbitmap</span><span class="p">))</span> <span class="p">{</span></div><div class='line' id='LC145'>		<span class="o">*</span><span class="n">page</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">page</span><span class="p">;</span></div><div class='line' id='LC146'>		<span class="o">*</span><span class="n">offset</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">offset</span><span class="p">;</span></div><div class='line' id='LC147'>		<span class="k">return</span> <span class="n">slindex</span><span class="p">;</span></div><div class='line' id='LC148'>	<span class="p">}</span></div><div class='line' id='LC149'><br/></div><div class='line' id='LC150'>	<span class="cm">/*</span></div><div class='line' id='LC151'><span class="cm">	 * No best-fit found. Search a bit further in bitmap for a free block.</span></div><div class='line' id='LC152'><span class="cm">	 * Second level bitmap consists of series of 32-bit chunks. Search</span></div><div class='line' id='LC153'><span class="cm">	 * further in the chunk where we expected a best-fit, starting from</span></div><div class='line' id='LC154'><span class="cm">	 * index location found above.</span></div><div class='line' id='LC155'><span class="cm">	 */</span></div><div class='line' id='LC156'>	<span class="n">slbitstart</span><span class="o">++</span><span class="p">;</span></div><div class='line' id='LC157'>	<span class="n">slbitmap</span> <span class="o">&gt;&gt;=</span> <span class="n">slbitstart</span><span class="p">;</span></div><div class='line' id='LC158'><br/></div><div class='line' id='LC159'>	<span class="cm">/* Skip this search if we were already at end of this bitmap chunk */</span></div><div class='line' id='LC160'>	<span class="k">if</span> <span class="p">((</span><span class="n">slbitstart</span> <span class="o">!=</span> <span class="n">BITS_PER_LONG</span><span class="p">)</span> <span class="o">&amp;&amp;</span> <span class="n">slbitmap</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC161'>		<span class="n">slindex</span> <span class="o">+=</span> <span class="n">__ffs</span><span class="p">(</span><span class="n">slbitmap</span><span class="p">)</span> <span class="o">+</span> <span class="mi">1</span><span class="p">;</span></div><div class='line' id='LC162'>		<span class="o">*</span><span class="n">page</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">page</span><span class="p">;</span></div><div class='line' id='LC163'>		<span class="o">*</span><span class="n">offset</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">offset</span><span class="p">;</span></div><div class='line' id='LC164'>		<span class="k">return</span> <span class="n">slindex</span><span class="p">;</span></div><div class='line' id='LC165'>	<span class="p">}</span></div><div class='line' id='LC166'><br/></div><div class='line' id='LC167'>	<span class="cm">/* Now do a full two-level bitmap search to find next nearest fit */</span></div><div class='line' id='LC168'>	<span class="n">flindex</span> <span class="o">=</span> <span class="n">slindex</span> <span class="o">/</span> <span class="n">BITS_PER_LONG</span><span class="p">;</span></div><div class='line' id='LC169'><br/></div><div class='line' id='LC170'>	<span class="n">flbitmap</span> <span class="o">=</span> <span class="p">(</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">flbitmap</span><span class="p">)</span> <span class="o">&gt;&gt;</span> <span class="p">(</span><span class="n">flindex</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC171'>	<span class="k">if</span> <span class="p">(</span><span class="o">!</span><span class="n">flbitmap</span><span class="p">)</span></div><div class='line' id='LC172'>		<span class="k">return</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC173'><br/></div><div class='line' id='LC174'>	<span class="n">flindex</span> <span class="o">+=</span> <span class="n">__ffs</span><span class="p">(</span><span class="n">flbitmap</span><span class="p">)</span> <span class="o">+</span> <span class="mi">1</span><span class="p">;</span></div><div class='line' id='LC175'>	<span class="n">slbitmap</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">slbitmap</span><span class="p">[</span><span class="n">flindex</span><span class="p">];</span></div><div class='line' id='LC176'>	<span class="n">slindex</span> <span class="o">=</span> <span class="p">(</span><span class="n">flindex</span> <span class="o">*</span> <span class="n">BITS_PER_LONG</span><span class="p">)</span> <span class="o">+</span> <span class="n">__ffs</span><span class="p">(</span><span class="n">slbitmap</span><span class="p">);</span></div><div class='line' id='LC177'>	<span class="o">*</span><span class="n">page</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">page</span><span class="p">;</span></div><div class='line' id='LC178'>	<span class="o">*</span><span class="n">offset</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">offset</span><span class="p">;</span></div><div class='line' id='LC179'><br/></div><div class='line' id='LC180'>	<span class="k">return</span> <span class="n">slindex</span><span class="p">;</span></div><div class='line' id='LC181'><span class="p">}</span></div><div class='line' id='LC182'><br/></div><div class='line' id='LC183'><span class="cm">/*</span></div><div class='line' id='LC184'><span class="cm"> * Insert block at &lt;page, offset&gt; in freelist of given pool.</span></div><div class='line' id='LC185'><span class="cm"> * freelist used depends on block size.</span></div><div class='line' id='LC186'><span class="cm"> */</span></div><div class='line' id='LC187'><span class="k">static</span> <span class="kt">void</span> <span class="nf">insert_block</span><span class="p">(</span><span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="n">pool</span><span class="p">,</span> <span class="k">struct</span> <span class="n">page</span> <span class="o">*</span><span class="n">page</span><span class="p">,</span> <span class="n">u32</span> <span class="n">offset</span><span class="p">,</span></div><div class='line' id='LC188'>			<span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">)</span></div><div class='line' id='LC189'><span class="p">{</span></div><div class='line' id='LC190'>	<span class="n">u32</span> <span class="n">flindex</span><span class="p">,</span> <span class="n">slindex</span><span class="p">;</span></div><div class='line' id='LC191'>	<span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">nextblock</span><span class="p">;</span></div><div class='line' id='LC192'><br/></div><div class='line' id='LC193'>	<span class="n">slindex</span> <span class="o">=</span> <span class="n">get_index_for_insert</span><span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span><span class="p">);</span></div><div class='line' id='LC194'>	<span class="n">flindex</span> <span class="o">=</span> <span class="n">slindex</span> <span class="o">/</span> <span class="n">BITS_PER_LONG</span><span class="p">;</span></div><div class='line' id='LC195'><br/></div><div class='line' id='LC196'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_page</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC197'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_offset</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC198'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_page</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">page</span><span class="p">;</span></div><div class='line' id='LC199'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_offset</span> <span class="o">=</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">offset</span><span class="p">;</span></div><div class='line' id='LC200'>	<span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">page</span> <span class="o">=</span> <span class="n">page</span><span class="p">;</span></div><div class='line' id='LC201'>	<span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">offset</span> <span class="o">=</span> <span class="n">offset</span><span class="p">;</span></div><div class='line' id='LC202'><br/></div><div class='line' id='LC203'>	<span class="k">if</span> <span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_page</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC204'>		<span class="n">nextblock</span> <span class="o">=</span> <span class="n">get_ptr_atomic</span><span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_page</span><span class="p">,</span></div><div class='line' id='LC205'>					<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_offset</span><span class="p">,</span> <span class="n">KM_USER1</span><span class="p">);</span></div><div class='line' id='LC206'>		<span class="n">nextblock</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_page</span> <span class="o">=</span> <span class="n">page</span><span class="p">;</span></div><div class='line' id='LC207'>		<span class="n">nextblock</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_offset</span> <span class="o">=</span> <span class="n">offset</span><span class="p">;</span></div><div class='line' id='LC208'>		<span class="n">put_ptr_atomic</span><span class="p">(</span><span class="n">nextblock</span><span class="p">,</span> <span class="n">KM_USER1</span><span class="p">);</span></div><div class='line' id='LC209'>		<span class="cm">/* If there was a next page then the free bits are set. */</span></div><div class='line' id='LC210'>		<span class="k">return</span><span class="p">;</span></div><div class='line' id='LC211'>	<span class="p">}</span></div><div class='line' id='LC212'><br/></div><div class='line' id='LC213'>	<span class="n">__set_bit</span><span class="p">(</span><span class="n">slindex</span> <span class="o">%</span> <span class="n">BITS_PER_LONG</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">slbitmap</span><span class="p">[</span><span class="n">flindex</span><span class="p">]);</span></div><div class='line' id='LC214'>	<span class="n">__set_bit</span><span class="p">(</span><span class="n">flindex</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">flbitmap</span><span class="p">);</span></div><div class='line' id='LC215'><span class="p">}</span></div><div class='line' id='LC216'><br/></div><div class='line' id='LC217'><span class="cm">/*</span></div><div class='line' id='LC218'><span class="cm"> * Remove block from freelist. Index &#39;slindex&#39; identifies the freelist.</span></div><div class='line' id='LC219'><span class="cm"> */</span></div><div class='line' id='LC220'><span class="k">static</span> <span class="kt">void</span> <span class="nf">remove_block</span><span class="p">(</span><span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="n">pool</span><span class="p">,</span> <span class="k">struct</span> <span class="n">page</span> <span class="o">*</span><span class="n">page</span><span class="p">,</span> <span class="n">u32</span> <span class="n">offset</span><span class="p">,</span></div><div class='line' id='LC221'>			<span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">,</span> <span class="n">u32</span> <span class="n">slindex</span><span class="p">)</span></div><div class='line' id='LC222'><span class="p">{</span></div><div class='line' id='LC223'>	<span class="n">u32</span> <span class="n">flindex</span> <span class="o">=</span> <span class="n">slindex</span> <span class="o">/</span> <span class="n">BITS_PER_LONG</span><span class="p">;</span></div><div class='line' id='LC224'>	<span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">tmpblock</span><span class="p">;</span></div><div class='line' id='LC225'><br/></div><div class='line' id='LC226'>	<span class="k">if</span> <span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_page</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC227'>		<span class="n">tmpblock</span> <span class="o">=</span> <span class="n">get_ptr_atomic</span><span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_page</span><span class="p">,</span></div><div class='line' id='LC228'>				<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_offset</span><span class="p">,</span> <span class="n">KM_USER1</span><span class="p">);</span></div><div class='line' id='LC229'>		<span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_page</span> <span class="o">=</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_page</span><span class="p">;</span></div><div class='line' id='LC230'>		<span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_offset</span> <span class="o">=</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_offset</span><span class="p">;</span></div><div class='line' id='LC231'>		<span class="n">put_ptr_atomic</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">KM_USER1</span><span class="p">);</span></div><div class='line' id='LC232'>	<span class="p">}</span></div><div class='line' id='LC233'><br/></div><div class='line' id='LC234'>	<span class="k">if</span> <span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_page</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC235'>		<span class="n">tmpblock</span> <span class="o">=</span> <span class="n">get_ptr_atomic</span><span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_page</span><span class="p">,</span></div><div class='line' id='LC236'>				<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_offset</span><span class="p">,</span> <span class="n">KM_USER1</span><span class="p">);</span></div><div class='line' id='LC237'>		<span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_page</span> <span class="o">=</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_page</span><span class="p">;</span></div><div class='line' id='LC238'>		<span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_offset</span> <span class="o">=</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_offset</span><span class="p">;</span></div><div class='line' id='LC239'>		<span class="n">put_ptr_atomic</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">KM_USER1</span><span class="p">);</span></div><div class='line' id='LC240'>	<span class="p">}</span></div><div class='line' id='LC241'><br/></div><div class='line' id='LC242'>	<span class="cm">/* Is this block is at the head of the freelist? */</span></div><div class='line' id='LC243'>	<span class="k">if</span> <span class="p">(</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">page</span> <span class="o">==</span> <span class="n">page</span></div><div class='line' id='LC244'>	   <span class="o">&amp;&amp;</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">offset</span> <span class="o">==</span> <span class="n">offset</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC245'><br/></div><div class='line' id='LC246'>		<span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">page</span> <span class="o">=</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_page</span><span class="p">;</span></div><div class='line' id='LC247'>		<span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">offset</span> <span class="o">=</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_offset</span><span class="p">;</span></div><div class='line' id='LC248'><br/></div><div class='line' id='LC249'>		<span class="k">if</span> <span class="p">(</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">page</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC250'>			<span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">tmpblock</span><span class="p">;</span></div><div class='line' id='LC251'>			<span class="n">tmpblock</span> <span class="o">=</span> <span class="n">get_ptr_atomic</span><span class="p">(</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">page</span><span class="p">,</span></div><div class='line' id='LC252'>					<span class="n">pool</span><span class="o">-&gt;</span><span class="n">freelist</span><span class="p">[</span><span class="n">slindex</span><span class="p">].</span><span class="n">offset</span><span class="p">,</span></div><div class='line' id='LC253'>					<span class="n">KM_USER1</span><span class="p">);</span></div><div class='line' id='LC254'>			<span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_page</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC255'>			<span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_offset</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC256'>			<span class="n">put_ptr_atomic</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">KM_USER1</span><span class="p">);</span></div><div class='line' id='LC257'>		<span class="p">}</span> <span class="k">else</span> <span class="p">{</span></div><div class='line' id='LC258'>			<span class="cm">/* This freelist bucket is empty */</span></div><div class='line' id='LC259'>			<span class="n">__clear_bit</span><span class="p">(</span><span class="n">slindex</span> <span class="o">%</span> <span class="n">BITS_PER_LONG</span><span class="p">,</span></div><div class='line' id='LC260'>				    <span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">slbitmap</span><span class="p">[</span><span class="n">flindex</span><span class="p">]);</span></div><div class='line' id='LC261'>			<span class="k">if</span> <span class="p">(</span><span class="o">!</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">slbitmap</span><span class="p">[</span><span class="n">flindex</span><span class="p">])</span></div><div class='line' id='LC262'>				<span class="n">__clear_bit</span><span class="p">(</span><span class="n">flindex</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">flbitmap</span><span class="p">);</span></div><div class='line' id='LC263'>		<span class="p">}</span></div><div class='line' id='LC264'>	<span class="p">}</span></div><div class='line' id='LC265'><br/></div><div class='line' id='LC266'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_page</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC267'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">prev_offset</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC268'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_page</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC269'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">link</span><span class="p">.</span><span class="n">next_offset</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC270'><span class="p">}</span></div><div class='line' id='LC271'><br/></div><div class='line' id='LC272'><span class="cm">/*</span></div><div class='line' id='LC273'><span class="cm"> * Allocate a page and add it to freelist of given pool.</span></div><div class='line' id='LC274'><span class="cm"> */</span></div><div class='line' id='LC275'><span class="k">static</span> <span class="kt">int</span> <span class="nf">grow_pool</span><span class="p">(</span><span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="n">pool</span><span class="p">,</span> <span class="n">gfp_t</span> <span class="n">flags</span><span class="p">)</span></div><div class='line' id='LC276'><span class="p">{</span></div><div class='line' id='LC277'>	<span class="k">struct</span> <span class="n">page</span> <span class="o">*</span><span class="n">page</span><span class="p">;</span></div><div class='line' id='LC278'>	<span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">;</span></div><div class='line' id='LC279'><br/></div><div class='line' id='LC280'>	<span class="n">page</span> <span class="o">=</span> <span class="n">alloc_page</span><span class="p">(</span><span class="n">flags</span><span class="p">);</span></div><div class='line' id='LC281'>	<span class="k">if</span> <span class="p">(</span><span class="n">unlikely</span><span class="p">(</span><span class="o">!</span><span class="n">page</span><span class="p">))</span></div><div class='line' id='LC282'>		<span class="k">return</span> <span class="o">-</span><span class="n">ENOMEM</span><span class="p">;</span></div><div class='line' id='LC283'><br/></div><div class='line' id='LC284'>	<span class="n">stat_inc</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">total_pages</span><span class="p">);</span></div><div class='line' id='LC285'><br/></div><div class='line' id='LC286'>	<span class="n">spin_lock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC287'>	<span class="n">block</span> <span class="o">=</span> <span class="n">get_ptr_atomic</span><span class="p">(</span><span class="n">page</span><span class="p">,</span> <span class="mi">0</span><span class="p">,</span> <span class="n">KM_USER0</span><span class="p">);</span></div><div class='line' id='LC288'><br/></div><div class='line' id='LC289'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">=</span> <span class="n">PAGE_SIZE</span> <span class="o">-</span> <span class="n">XV_ALIGN</span><span class="p">;</span></div><div class='line' id='LC290'>	<span class="n">set_flag</span><span class="p">(</span><span class="n">block</span><span class="p">,</span> <span class="n">BLOCK_FREE</span><span class="p">);</span></div><div class='line' id='LC291'>	<span class="n">clear_flag</span><span class="p">(</span><span class="n">block</span><span class="p">,</span> <span class="n">PREV_FREE</span><span class="p">);</span></div><div class='line' id='LC292'>	<span class="n">set_blockprev</span><span class="p">(</span><span class="n">block</span><span class="p">,</span> <span class="mi">0</span><span class="p">);</span></div><div class='line' id='LC293'><br/></div><div class='line' id='LC294'>	<span class="n">insert_block</span><span class="p">(</span><span class="n">pool</span><span class="p">,</span> <span class="n">page</span><span class="p">,</span> <span class="mi">0</span><span class="p">,</span> <span class="n">block</span><span class="p">);</span></div><div class='line' id='LC295'><br/></div><div class='line' id='LC296'>	<span class="n">put_ptr_atomic</span><span class="p">(</span><span class="n">block</span><span class="p">,</span> <span class="n">KM_USER0</span><span class="p">);</span></div><div class='line' id='LC297'>	<span class="n">spin_unlock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC298'><br/></div><div class='line' id='LC299'>	<span class="k">return</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC300'><span class="p">}</span></div><div class='line' id='LC301'><br/></div><div class='line' id='LC302'><span class="cm">/*</span></div><div class='line' id='LC303'><span class="cm"> * Create a memory pool. Allocates freelist, bitmaps and other</span></div><div class='line' id='LC304'><span class="cm"> * per-pool metadata.</span></div><div class='line' id='LC305'><span class="cm"> */</span></div><div class='line' id='LC306'><span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="nf">xv_create_pool</span><span class="p">(</span><span class="kt">void</span><span class="p">)</span></div><div class='line' id='LC307'><span class="p">{</span></div><div class='line' id='LC308'>	<span class="n">u32</span> <span class="n">ovhd_size</span><span class="p">;</span></div><div class='line' id='LC309'>	<span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="n">pool</span><span class="p">;</span></div><div class='line' id='LC310'><br/></div><div class='line' id='LC311'>	<span class="n">ovhd_size</span> <span class="o">=</span> <span class="n">roundup</span><span class="p">(</span><span class="k">sizeof</span><span class="p">(</span><span class="o">*</span><span class="n">pool</span><span class="p">),</span> <span class="n">PAGE_SIZE</span><span class="p">);</span></div><div class='line' id='LC312'>	<span class="n">pool</span> <span class="o">=</span> <span class="n">kzalloc</span><span class="p">(</span><span class="n">ovhd_size</span><span class="p">,</span> <span class="n">GFP_KERNEL</span><span class="p">);</span></div><div class='line' id='LC313'>	<span class="k">if</span> <span class="p">(</span><span class="o">!</span><span class="n">pool</span><span class="p">)</span></div><div class='line' id='LC314'>		<span class="k">return</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC315'><br/></div><div class='line' id='LC316'>	<span class="n">spin_lock_init</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC317'><br/></div><div class='line' id='LC318'>	<span class="k">return</span> <span class="n">pool</span><span class="p">;</span></div><div class='line' id='LC319'><span class="p">}</span></div><div class='line' id='LC320'><span class="n">EXPORT_SYMBOL_GPL</span><span class="p">(</span><span class="n">xv_create_pool</span><span class="p">);</span></div><div class='line' id='LC321'><br/></div><div class='line' id='LC322'><span class="kt">void</span> <span class="nf">xv_destroy_pool</span><span class="p">(</span><span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="n">pool</span><span class="p">)</span></div><div class='line' id='LC323'><span class="p">{</span></div><div class='line' id='LC324'>	<span class="n">kfree</span><span class="p">(</span><span class="n">pool</span><span class="p">);</span></div><div class='line' id='LC325'><span class="p">}</span></div><div class='line' id='LC326'><span class="n">EXPORT_SYMBOL_GPL</span><span class="p">(</span><span class="n">xv_destroy_pool</span><span class="p">);</span></div><div class='line' id='LC327'><br/></div><div class='line' id='LC328'><span class="cm">/**</span></div><div class='line' id='LC329'><span class="cm"> * xv_malloc - Allocate block of given size from pool.</span></div><div class='line' id='LC330'><span class="cm"> * @pool: pool to allocate from</span></div><div class='line' id='LC331'><span class="cm"> * @size: size of block to allocate</span></div><div class='line' id='LC332'><span class="cm"> * @page: page no. that holds the object</span></div><div class='line' id='LC333'><span class="cm"> * @offset: location of object within page</span></div><div class='line' id='LC334'><span class="cm"> *</span></div><div class='line' id='LC335'><span class="cm"> * On success, &lt;page, offset&gt; identifies block allocated</span></div><div class='line' id='LC336'><span class="cm"> * and 0 is returned. On failure, &lt;page, offset&gt; is set to</span></div><div class='line' id='LC337'><span class="cm"> * 0 and -ENOMEM is returned.</span></div><div class='line' id='LC338'><span class="cm"> *</span></div><div class='line' id='LC339'><span class="cm"> * Allocation requests with size &gt; XV_MAX_ALLOC_SIZE will fail.</span></div><div class='line' id='LC340'><span class="cm"> */</span></div><div class='line' id='LC341'><span class="kt">int</span> <span class="nf">xv_malloc</span><span class="p">(</span><span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="n">pool</span><span class="p">,</span> <span class="n">u32</span> <span class="n">size</span><span class="p">,</span> <span class="k">struct</span> <span class="n">page</span> <span class="o">**</span><span class="n">page</span><span class="p">,</span></div><div class='line' id='LC342'>		<span class="n">u32</span> <span class="o">*</span><span class="n">offset</span><span class="p">,</span> <span class="n">gfp_t</span> <span class="n">flags</span><span class="p">)</span></div><div class='line' id='LC343'><span class="p">{</span></div><div class='line' id='LC344'>	<span class="kt">int</span> <span class="n">error</span><span class="p">;</span></div><div class='line' id='LC345'>	<span class="n">u32</span> <span class="n">index</span><span class="p">,</span> <span class="n">tmpsize</span><span class="p">,</span> <span class="n">origsize</span><span class="p">,</span> <span class="n">tmpoffset</span><span class="p">;</span></div><div class='line' id='LC346'>	<span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">,</span> <span class="o">*</span><span class="n">tmpblock</span><span class="p">;</span></div><div class='line' id='LC347'><br/></div><div class='line' id='LC348'>	<span class="o">*</span><span class="n">page</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC349'>	<span class="o">*</span><span class="n">offset</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC350'>	<span class="n">origsize</span> <span class="o">=</span> <span class="n">size</span><span class="p">;</span></div><div class='line' id='LC351'><br/></div><div class='line' id='LC352'>	<span class="k">if</span> <span class="p">(</span><span class="n">unlikely</span><span class="p">(</span><span class="o">!</span><span class="n">size</span> <span class="o">||</span> <span class="n">size</span> <span class="o">&gt;</span> <span class="n">XV_MAX_ALLOC_SIZE</span><span class="p">))</span></div><div class='line' id='LC353'>		<span class="k">return</span> <span class="o">-</span><span class="n">ENOMEM</span><span class="p">;</span></div><div class='line' id='LC354'><br/></div><div class='line' id='LC355'>	<span class="n">size</span> <span class="o">=</span> <span class="n">ALIGN</span><span class="p">(</span><span class="n">size</span><span class="p">,</span> <span class="n">XV_ALIGN</span><span class="p">);</span></div><div class='line' id='LC356'><br/></div><div class='line' id='LC357'>	<span class="n">spin_lock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC358'><br/></div><div class='line' id='LC359'>	<span class="n">index</span> <span class="o">=</span> <span class="n">find_block</span><span class="p">(</span><span class="n">pool</span><span class="p">,</span> <span class="n">size</span><span class="p">,</span> <span class="n">page</span><span class="p">,</span> <span class="n">offset</span><span class="p">);</span></div><div class='line' id='LC360'><br/></div><div class='line' id='LC361'>	<span class="k">if</span> <span class="p">(</span><span class="o">!*</span><span class="n">page</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC362'>		<span class="n">spin_unlock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC363'>		<span class="k">if</span> <span class="p">(</span><span class="n">flags</span> <span class="o">&amp;</span> <span class="n">GFP_NOWAIT</span><span class="p">)</span></div><div class='line' id='LC364'>			<span class="k">return</span> <span class="o">-</span><span class="n">ENOMEM</span><span class="p">;</span></div><div class='line' id='LC365'>		<span class="n">error</span> <span class="o">=</span> <span class="n">grow_pool</span><span class="p">(</span><span class="n">pool</span><span class="p">,</span> <span class="n">flags</span><span class="p">);</span></div><div class='line' id='LC366'>		<span class="k">if</span> <span class="p">(</span><span class="n">unlikely</span><span class="p">(</span><span class="n">error</span><span class="p">))</span></div><div class='line' id='LC367'>			<span class="k">return</span> <span class="n">error</span><span class="p">;</span></div><div class='line' id='LC368'><br/></div><div class='line' id='LC369'>		<span class="n">spin_lock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC370'>		<span class="n">index</span> <span class="o">=</span> <span class="n">find_block</span><span class="p">(</span><span class="n">pool</span><span class="p">,</span> <span class="n">size</span><span class="p">,</span> <span class="n">page</span><span class="p">,</span> <span class="n">offset</span><span class="p">);</span></div><div class='line' id='LC371'>	<span class="p">}</span></div><div class='line' id='LC372'><br/></div><div class='line' id='LC373'>	<span class="k">if</span> <span class="p">(</span><span class="o">!*</span><span class="n">page</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC374'>		<span class="n">spin_unlock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC375'>		<span class="k">return</span> <span class="o">-</span><span class="n">ENOMEM</span><span class="p">;</span></div><div class='line' id='LC376'>	<span class="p">}</span></div><div class='line' id='LC377'><br/></div><div class='line' id='LC378'>	<span class="n">block</span> <span class="o">=</span> <span class="n">get_ptr_atomic</span><span class="p">(</span><span class="o">*</span><span class="n">page</span><span class="p">,</span> <span class="o">*</span><span class="n">offset</span><span class="p">,</span> <span class="n">KM_USER0</span><span class="p">);</span></div><div class='line' id='LC379'><br/></div><div class='line' id='LC380'>	<span class="n">remove_block</span><span class="p">(</span><span class="n">pool</span><span class="p">,</span> <span class="o">*</span><span class="n">page</span><span class="p">,</span> <span class="o">*</span><span class="n">offset</span><span class="p">,</span> <span class="n">block</span><span class="p">,</span> <span class="n">index</span><span class="p">);</span></div><div class='line' id='LC381'><br/></div><div class='line' id='LC382'>	<span class="cm">/* Split the block if required */</span></div><div class='line' id='LC383'>	<span class="n">tmpoffset</span> <span class="o">=</span> <span class="o">*</span><span class="n">offset</span> <span class="o">+</span> <span class="n">size</span> <span class="o">+</span> <span class="n">XV_ALIGN</span><span class="p">;</span></div><div class='line' id='LC384'>	<span class="n">tmpsize</span> <span class="o">=</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">-</span> <span class="n">size</span><span class="p">;</span></div><div class='line' id='LC385'>	<span class="n">tmpblock</span> <span class="o">=</span> <span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="p">)((</span><span class="kt">char</span> <span class="o">*</span><span class="p">)</span><span class="n">block</span> <span class="o">+</span> <span class="n">size</span> <span class="o">+</span> <span class="n">XV_ALIGN</span><span class="p">);</span></div><div class='line' id='LC386'>	<span class="k">if</span> <span class="p">(</span><span class="n">tmpsize</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC387'>		<span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">=</span> <span class="n">tmpsize</span> <span class="o">-</span> <span class="n">XV_ALIGN</span><span class="p">;</span></div><div class='line' id='LC388'>		<span class="n">set_flag</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">BLOCK_FREE</span><span class="p">);</span></div><div class='line' id='LC389'>		<span class="n">clear_flag</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">PREV_FREE</span><span class="p">);</span></div><div class='line' id='LC390'><br/></div><div class='line' id='LC391'>		<span class="n">set_blockprev</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="o">*</span><span class="n">offset</span><span class="p">);</span></div><div class='line' id='LC392'>		<span class="k">if</span> <span class="p">(</span><span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">&gt;=</span> <span class="n">XV_MIN_ALLOC_SIZE</span><span class="p">)</span></div><div class='line' id='LC393'>			<span class="n">insert_block</span><span class="p">(</span><span class="n">pool</span><span class="p">,</span> <span class="o">*</span><span class="n">page</span><span class="p">,</span> <span class="n">tmpoffset</span><span class="p">,</span> <span class="n">tmpblock</span><span class="p">);</span></div><div class='line' id='LC394'><br/></div><div class='line' id='LC395'>		<span class="k">if</span> <span class="p">(</span><span class="n">tmpoffset</span> <span class="o">+</span> <span class="n">XV_ALIGN</span> <span class="o">+</span> <span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">!=</span> <span class="n">PAGE_SIZE</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC396'>			<span class="n">tmpblock</span> <span class="o">=</span> <span class="n">BLOCK_NEXT</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">);</span></div><div class='line' id='LC397'>			<span class="n">set_blockprev</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">tmpoffset</span><span class="p">);</span></div><div class='line' id='LC398'>		<span class="p">}</span></div><div class='line' id='LC399'>	<span class="p">}</span> <span class="k">else</span> <span class="p">{</span></div><div class='line' id='LC400'>		<span class="cm">/* This block is exact fit */</span></div><div class='line' id='LC401'>		<span class="k">if</span> <span class="p">(</span><span class="n">tmpoffset</span> <span class="o">!=</span> <span class="n">PAGE_SIZE</span><span class="p">)</span></div><div class='line' id='LC402'>			<span class="n">clear_flag</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">PREV_FREE</span><span class="p">);</span></div><div class='line' id='LC403'>	<span class="p">}</span></div><div class='line' id='LC404'><br/></div><div class='line' id='LC405'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">=</span> <span class="n">origsize</span><span class="p">;</span></div><div class='line' id='LC406'>	<span class="n">clear_flag</span><span class="p">(</span><span class="n">block</span><span class="p">,</span> <span class="n">BLOCK_FREE</span><span class="p">);</span></div><div class='line' id='LC407'><br/></div><div class='line' id='LC408'>	<span class="n">put_ptr_atomic</span><span class="p">(</span><span class="n">block</span><span class="p">,</span> <span class="n">KM_USER0</span><span class="p">);</span></div><div class='line' id='LC409'>	<span class="n">spin_unlock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC410'><br/></div><div class='line' id='LC411'>	<span class="o">*</span><span class="n">offset</span> <span class="o">+=</span> <span class="n">XV_ALIGN</span><span class="p">;</span></div><div class='line' id='LC412'><br/></div><div class='line' id='LC413'>	<span class="k">return</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC414'><span class="p">}</span></div><div class='line' id='LC415'><span class="n">EXPORT_SYMBOL_GPL</span><span class="p">(</span><span class="n">xv_malloc</span><span class="p">);</span></div><div class='line' id='LC416'><br/></div><div class='line' id='LC417'><span class="cm">/*</span></div><div class='line' id='LC418'><span class="cm"> * Free block identified with &lt;page, offset&gt;</span></div><div class='line' id='LC419'><span class="cm"> */</span></div><div class='line' id='LC420'><span class="kt">void</span> <span class="nf">xv_free</span><span class="p">(</span><span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="n">pool</span><span class="p">,</span> <span class="k">struct</span> <span class="n">page</span> <span class="o">*</span><span class="n">page</span><span class="p">,</span> <span class="n">u32</span> <span class="n">offset</span><span class="p">)</span></div><div class='line' id='LC421'><span class="p">{</span></div><div class='line' id='LC422'>	<span class="kt">void</span> <span class="o">*</span><span class="n">page_start</span><span class="p">;</span></div><div class='line' id='LC423'>	<span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">block</span><span class="p">,</span> <span class="o">*</span><span class="n">tmpblock</span><span class="p">;</span></div><div class='line' id='LC424'><br/></div><div class='line' id='LC425'>	<span class="n">offset</span> <span class="o">-=</span> <span class="n">XV_ALIGN</span><span class="p">;</span></div><div class='line' id='LC426'><br/></div><div class='line' id='LC427'>	<span class="n">spin_lock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC428'><br/></div><div class='line' id='LC429'>	<span class="n">page_start</span> <span class="o">=</span> <span class="n">get_ptr_atomic</span><span class="p">(</span><span class="n">page</span><span class="p">,</span> <span class="mi">0</span><span class="p">,</span> <span class="n">KM_USER0</span><span class="p">);</span></div><div class='line' id='LC430'>	<span class="n">block</span> <span class="o">=</span> <span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="p">)((</span><span class="kt">char</span> <span class="o">*</span><span class="p">)</span><span class="n">page_start</span> <span class="o">+</span> <span class="n">offset</span><span class="p">);</span></div><div class='line' id='LC431'><br/></div><div class='line' id='LC432'>	<span class="cm">/* Catch double free bugs */</span></div><div class='line' id='LC433'>	<span class="n">BUG_ON</span><span class="p">(</span><span class="n">test_flag</span><span class="p">(</span><span class="n">block</span><span class="p">,</span> <span class="n">BLOCK_FREE</span><span class="p">));</span></div><div class='line' id='LC434'><br/></div><div class='line' id='LC435'>	<span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">=</span> <span class="n">ALIGN</span><span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span><span class="p">,</span> <span class="n">XV_ALIGN</span><span class="p">);</span></div><div class='line' id='LC436'><br/></div><div class='line' id='LC437'>	<span class="n">tmpblock</span> <span class="o">=</span> <span class="n">BLOCK_NEXT</span><span class="p">(</span><span class="n">block</span><span class="p">);</span></div><div class='line' id='LC438'>	<span class="k">if</span> <span class="p">(</span><span class="n">offset</span> <span class="o">+</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">+</span> <span class="n">XV_ALIGN</span> <span class="o">==</span> <span class="n">PAGE_SIZE</span><span class="p">)</span></div><div class='line' id='LC439'>		<span class="n">tmpblock</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC440'><br/></div><div class='line' id='LC441'>	<span class="cm">/* Merge next block if its free */</span></div><div class='line' id='LC442'>	<span class="k">if</span> <span class="p">(</span><span class="n">tmpblock</span> <span class="o">&amp;&amp;</span> <span class="n">test_flag</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">BLOCK_FREE</span><span class="p">))</span> <span class="p">{</span></div><div class='line' id='LC443'>		<span class="cm">/*</span></div><div class='line' id='LC444'><span class="cm">		 * Blocks smaller than XV_MIN_ALLOC_SIZE</span></div><div class='line' id='LC445'><span class="cm">		 * are not inserted in any free list.</span></div><div class='line' id='LC446'><span class="cm">		 */</span></div><div class='line' id='LC447'>		<span class="k">if</span> <span class="p">(</span><span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">&gt;=</span> <span class="n">XV_MIN_ALLOC_SIZE</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC448'>			<span class="n">remove_block</span><span class="p">(</span><span class="n">pool</span><span class="p">,</span> <span class="n">page</span><span class="p">,</span></div><div class='line' id='LC449'>				    <span class="n">offset</span> <span class="o">+</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">+</span> <span class="n">XV_ALIGN</span><span class="p">,</span> <span class="n">tmpblock</span><span class="p">,</span></div><div class='line' id='LC450'>				    <span class="n">get_index_for_insert</span><span class="p">(</span><span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span><span class="p">));</span></div><div class='line' id='LC451'>		<span class="p">}</span></div><div class='line' id='LC452'>		<span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">+=</span> <span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">+</span> <span class="n">XV_ALIGN</span><span class="p">;</span></div><div class='line' id='LC453'>	<span class="p">}</span></div><div class='line' id='LC454'><br/></div><div class='line' id='LC455'>	<span class="cm">/* Merge previous block if its free */</span></div><div class='line' id='LC456'>	<span class="k">if</span> <span class="p">(</span><span class="n">test_flag</span><span class="p">(</span><span class="n">block</span><span class="p">,</span> <span class="n">PREV_FREE</span><span class="p">))</span> <span class="p">{</span></div><div class='line' id='LC457'>		<span class="n">tmpblock</span> <span class="o">=</span> <span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="p">)((</span><span class="kt">char</span> <span class="o">*</span><span class="p">)(</span><span class="n">page_start</span><span class="p">)</span> <span class="o">+</span></div><div class='line' id='LC458'>						<span class="n">get_blockprev</span><span class="p">(</span><span class="n">block</span><span class="p">));</span></div><div class='line' id='LC459'>		<span class="n">offset</span> <span class="o">=</span> <span class="n">offset</span> <span class="o">-</span> <span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">-</span> <span class="n">XV_ALIGN</span><span class="p">;</span></div><div class='line' id='LC460'><br/></div><div class='line' id='LC461'>		<span class="k">if</span> <span class="p">(</span><span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">&gt;=</span> <span class="n">XV_MIN_ALLOC_SIZE</span><span class="p">)</span></div><div class='line' id='LC462'>			<span class="n">remove_block</span><span class="p">(</span><span class="n">pool</span><span class="p">,</span> <span class="n">page</span><span class="p">,</span> <span class="n">offset</span><span class="p">,</span> <span class="n">tmpblock</span><span class="p">,</span></div><div class='line' id='LC463'>				    <span class="n">get_index_for_insert</span><span class="p">(</span><span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span><span class="p">));</span></div><div class='line' id='LC464'><br/></div><div class='line' id='LC465'>		<span class="n">tmpblock</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">+=</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">+</span> <span class="n">XV_ALIGN</span><span class="p">;</span></div><div class='line' id='LC466'>		<span class="n">block</span> <span class="o">=</span> <span class="n">tmpblock</span><span class="p">;</span></div><div class='line' id='LC467'>	<span class="p">}</span></div><div class='line' id='LC468'><br/></div><div class='line' id='LC469'>	<span class="cm">/* No used objects in this page. Free it. */</span></div><div class='line' id='LC470'>	<span class="k">if</span> <span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">==</span> <span class="n">PAGE_SIZE</span> <span class="o">-</span> <span class="n">XV_ALIGN</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC471'>		<span class="n">put_ptr_atomic</span><span class="p">(</span><span class="n">page_start</span><span class="p">,</span> <span class="n">KM_USER0</span><span class="p">);</span></div><div class='line' id='LC472'>		<span class="n">spin_unlock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC473'><br/></div><div class='line' id='LC474'>		<span class="n">__free_page</span><span class="p">(</span><span class="n">page</span><span class="p">);</span></div><div class='line' id='LC475'>		<span class="n">stat_dec</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">total_pages</span><span class="p">);</span></div><div class='line' id='LC476'>		<span class="k">return</span><span class="p">;</span></div><div class='line' id='LC477'>	<span class="p">}</span></div><div class='line' id='LC478'><br/></div><div class='line' id='LC479'>	<span class="n">set_flag</span><span class="p">(</span><span class="n">block</span><span class="p">,</span> <span class="n">BLOCK_FREE</span><span class="p">);</span></div><div class='line' id='LC480'>	<span class="k">if</span> <span class="p">(</span><span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">&gt;=</span> <span class="n">XV_MIN_ALLOC_SIZE</span><span class="p">)</span></div><div class='line' id='LC481'>		<span class="n">insert_block</span><span class="p">(</span><span class="n">pool</span><span class="p">,</span> <span class="n">page</span><span class="p">,</span> <span class="n">offset</span><span class="p">,</span> <span class="n">block</span><span class="p">);</span></div><div class='line' id='LC482'><br/></div><div class='line' id='LC483'>	<span class="k">if</span> <span class="p">(</span><span class="n">offset</span> <span class="o">+</span> <span class="n">block</span><span class="o">-&gt;</span><span class="n">size</span> <span class="o">+</span> <span class="n">XV_ALIGN</span> <span class="o">!=</span> <span class="n">PAGE_SIZE</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC484'>		<span class="n">tmpblock</span> <span class="o">=</span> <span class="n">BLOCK_NEXT</span><span class="p">(</span><span class="n">block</span><span class="p">);</span></div><div class='line' id='LC485'>		<span class="n">set_flag</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">PREV_FREE</span><span class="p">);</span></div><div class='line' id='LC486'>		<span class="n">set_blockprev</span><span class="p">(</span><span class="n">tmpblock</span><span class="p">,</span> <span class="n">offset</span><span class="p">);</span></div><div class='line' id='LC487'>	<span class="p">}</span></div><div class='line' id='LC488'><br/></div><div class='line' id='LC489'>	<span class="n">put_ptr_atomic</span><span class="p">(</span><span class="n">page_start</span><span class="p">,</span> <span class="n">KM_USER0</span><span class="p">);</span></div><div class='line' id='LC490'>	<span class="n">spin_unlock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">pool</span><span class="o">-&gt;</span><span class="n">lock</span><span class="p">);</span></div><div class='line' id='LC491'><span class="p">}</span></div><div class='line' id='LC492'><span class="n">EXPORT_SYMBOL_GPL</span><span class="p">(</span><span class="n">xv_free</span><span class="p">);</span></div><div class='line' id='LC493'><br/></div><div class='line' id='LC494'><span class="n">u32</span> <span class="nf">xv_get_object_size</span><span class="p">(</span><span class="kt">void</span> <span class="o">*</span><span class="n">obj</span><span class="p">)</span></div><div class='line' id='LC495'><span class="p">{</span></div><div class='line' id='LC496'>	<span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="n">blk</span><span class="p">;</span></div><div class='line' id='LC497'><br/></div><div class='line' id='LC498'>	<span class="n">blk</span> <span class="o">=</span> <span class="p">(</span><span class="k">struct</span> <span class="n">block_header</span> <span class="o">*</span><span class="p">)((</span><span class="kt">char</span> <span class="o">*</span><span class="p">)(</span><span class="n">obj</span><span class="p">)</span> <span class="o">-</span> <span class="n">XV_ALIGN</span><span class="p">);</span></div><div class='line' id='LC499'>	<span class="k">return</span> <span class="n">blk</span><span class="o">-&gt;</span><span class="n">size</span><span class="p">;</span></div><div class='line' id='LC500'><span class="p">}</span></div><div class='line' id='LC501'><span class="n">EXPORT_SYMBOL_GPL</span><span class="p">(</span><span class="n">xv_get_object_size</span><span class="p">);</span></div><div class='line' id='LC502'><br/></div><div class='line' id='LC503'><span class="cm">/*</span></div><div class='line' id='LC504'><span class="cm"> * Returns total memory used by allocator (userdata + metadata)</span></div><div class='line' id='LC505'><span class="cm"> */</span></div><div class='line' id='LC506'><span class="n">u64</span> <span class="nf">xv_get_total_size_bytes</span><span class="p">(</span><span class="k">struct</span> <span class="n">xv_pool</span> <span class="o">*</span><span class="n">pool</span><span class="p">)</span></div><div class='line' id='LC507'><span class="p">{</span></div><div class='line' id='LC508'>	<span class="k">return</span> <span class="n">pool</span><span class="o">-&gt;</span><span class="n">total_pages</span> <span class="o">&lt;&lt;</span> <span class="n">PAGE_SHIFT</span><span class="p">;</span></div><div class='line' id='LC509'><span class="p">}</span></div><div class='line' id='LC510'><span class="n">EXPORT_SYMBOL_GPL</span><span class="p">(</span><span class="n">xv_get_total_size_bytes</span><span class="p">);</span></div></pre></div>
          </td>
        </tr>
      </table>
  </div>

          </div>
        </div>
      </div>

      <a href="#jump-to-line" rel="facebox" data-hotkey="l" class="js-jump-to-line" style="display:none">Jump to Line</a>
      <div id="jump-to-line" style="display:none">
        <h2>Jump to Line</h2>
        <form accept-charset="UTF-8" class="js-jump-to-line-form">
          <input class="textfield js-jump-to-line-field" type="text">
          <div class="full-button">
            <button type="submit" class="classy">
              Go
            </button>
          </div>
        </form>
      </div>

    </div>
</div>

<div class="frame frame-loading large-loading-area" style="display:none;">
  <img src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-128.gif?1347543527" height="64" width="64">
</div>

        </div>
      </div>
      <div class="context-overlay"></div>
    </div>

      <div id="footer-push"></div><!-- hack for sticky footer -->
    </div><!-- end of wrapper - hack for sticky footer -->

      <!-- footer -->
      <div id="footer">
  <div class="container clearfix">

      <dl class="footer_nav">
        <dt>GitHub</dt>
        <dd><a href="https://github.com/about">About us</a></dd>
        <dd><a href="https://github.com/blog">Blog</a></dd>
        <dd><a href="https://github.com/contact">Contact &amp; support</a></dd>
        <dd><a href="http://enterprise.github.com/">GitHub Enterprise</a></dd>
        <dd><a href="http://status.github.com/">Site status</a></dd>
      </dl>

      <dl class="footer_nav">
        <dt>Applications</dt>
        <dd><a href="http://mac.github.com/">GitHub for Mac</a></dd>
        <dd><a href="http://windows.github.com/">GitHub for Windows</a></dd>
        <dd><a href="http://eclipse.github.com/">GitHub for Eclipse</a></dd>
        <dd><a href="http://mobile.github.com/">GitHub mobile apps</a></dd>
      </dl>

      <dl class="footer_nav">
        <dt>Services</dt>
        <dd><a href="http://get.gaug.es/">Gauges: Web analytics</a></dd>
        <dd><a href="http://speakerdeck.com">Speaker Deck: Presentations</a></dd>
        <dd><a href="https://gist.github.com">Gist: Code snippets</a></dd>
        <dd><a href="http://jobs.github.com/">Job board</a></dd>
      </dl>

      <dl class="footer_nav">
        <dt>Documentation</dt>
        <dd><a href="http://help.github.com/">GitHub Help</a></dd>
        <dd><a href="http://developer.github.com/">Developer API</a></dd>
        <dd><a href="http://github.github.com/github-flavored-markdown/">GitHub Flavored Markdown</a></dd>
        <dd><a href="http://pages.github.com/">GitHub Pages</a></dd>
      </dl>

      <dl class="footer_nav">
        <dt>More</dt>
        <dd><a href="http://training.github.com/">Training</a></dd>
        <dd><a href="https://github.com/edu">Students &amp; teachers</a></dd>
        <dd><a href="http://shop.github.com">The Shop</a></dd>
        <dd><a href="http://octodex.github.com/">The Octodex</a></dd>
      </dl>

      <hr class="footer-divider">


    <p class="right">&copy; 2012 <span title="0.08921s from fe18.rs.github.com">GitHub</span> Inc. All rights reserved.</p>
    <a class="left" href="https://github.com/">
      <span class="mega-icon mega-icon-invertocat"></span>
    </a>
    <ul id="legal">
        <li><a href="https://github.com/site/terms">Terms of Service</a></li>
        <li><a href="https://github.com/site/privacy">Privacy</a></li>
        <li><a href="https://github.com/security">Security</a></li>
    </ul>

  </div><!-- /.container -->

</div><!-- /.#footer -->


    

<div id="keyboard_shortcuts_pane" class="instapaper_ignore readability-extra" style="display:none">
  <h2>Keyboard Shortcuts <small><a href="#" class="js-see-all-keyboard-shortcuts">(see all)</a></small></h2>

  <div class="columns threecols">
    <div class="column first">
      <h3>Site wide shortcuts</h3>
      <dl class="keyboard-mappings">
        <dt>s</dt>
        <dd>Focus command bar</dd>
      </dl>
      <dl class="keyboard-mappings">
        <dt>?</dt>
        <dd>Bring up this help dialog</dd>
      </dl>
    </div><!-- /.column.first -->

    <div class="column middle" style='display:none'>
      <h3>Commit list</h3>
      <dl class="keyboard-mappings">
        <dt>j</dt>
        <dd>Move selection down</dd>
      </dl>
      <dl class="keyboard-mappings">
        <dt>k</dt>
        <dd>Move selection up</dd>
      </dl>
      <dl class="keyboard-mappings">
        <dt>c <em>or</em> o <em>or</em> enter</dt>
        <dd>Open commit</dd>
      </dl>
      <dl class="keyboard-mappings">
        <dt>y</dt>
        <dd>Expand URL to its canonical form</dd>
      </dl>
    </div><!-- /.column.first -->

    <div class="column last js-hidden-pane" style='display:none'>
      <h3>Pull request list</h3>
      <dl class="keyboard-mappings">
        <dt>j</dt>
        <dd>Move selection down</dd>
      </dl>
      <dl class="keyboard-mappings">
        <dt>k</dt>
        <dd>Move selection up</dd>
      </dl>
      <dl class="keyboard-mappings">
        <dt>o <em>or</em> enter</dt>
        <dd>Open issue</dd>
      </dl>
      <dl class="keyboard-mappings">
        <dt><span class="platform-mac">⌘</span><span class="platform-other">ctrl</span> <em>+</em> enter</dt>
        <dd>Submit comment</dd>
      </dl>
      <dl class="keyboard-mappings">
        <dt><span class="platform-mac">⌘</span><span class="platform-other">ctrl</span> <em>+</em> shift p</dt>
        <dd>Preview comment</dd>
      </dl>
    </div><!-- /.columns.last -->

  </div><!-- /.columns.equacols -->

  <div class="js-hidden-pane" style='display:none'>
    <div class="rule"></div>

    <h3>Issues</h3>

    <div class="columns threecols">
      <div class="column first">
        <dl class="keyboard-mappings">
          <dt>j</dt>
          <dd>Move selection down</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>k</dt>
          <dd>Move selection up</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>x</dt>
          <dd>Toggle selection</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>o <em>or</em> enter</dt>
          <dd>Open issue</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt><span class="platform-mac">⌘</span><span class="platform-other">ctrl</span> <em>+</em> enter</dt>
          <dd>Submit comment</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt><span class="platform-mac">⌘</span><span class="platform-other">ctrl</span> <em>+</em> shift p</dt>
          <dd>Preview comment</dd>
        </dl>
      </div><!-- /.column.first -->
      <div class="column last">
        <dl class="keyboard-mappings">
          <dt>c</dt>
          <dd>Create issue</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>l</dt>
          <dd>Create label</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>i</dt>
          <dd>Back to inbox</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>u</dt>
          <dd>Back to issues</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>/</dt>
          <dd>Focus issues search</dd>
        </dl>
      </div>
    </div>
  </div>

  <div class="js-hidden-pane" style='display:none'>
    <div class="rule"></div>

    <h3>Issues Dashboard</h3>

    <div class="columns threecols">
      <div class="column first">
        <dl class="keyboard-mappings">
          <dt>j</dt>
          <dd>Move selection down</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>k</dt>
          <dd>Move selection up</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>o <em>or</em> enter</dt>
          <dd>Open issue</dd>
        </dl>
      </div><!-- /.column.first -->
    </div>
  </div>

  <div class="js-hidden-pane" style='display:none'>
    <div class="rule"></div>

    <h3>Network Graph</h3>
    <div class="columns equacols">
      <div class="column first">
        <dl class="keyboard-mappings">
          <dt><span class="badmono">←</span> <em>or</em> h</dt>
          <dd>Scroll left</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt><span class="badmono">→</span> <em>or</em> l</dt>
          <dd>Scroll right</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt><span class="badmono">↑</span> <em>or</em> k</dt>
          <dd>Scroll up</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt><span class="badmono">↓</span> <em>or</em> j</dt>
          <dd>Scroll down</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>t</dt>
          <dd>Toggle visibility of head labels</dd>
        </dl>
      </div><!-- /.column.first -->
      <div class="column last">
        <dl class="keyboard-mappings">
          <dt>shift <span class="badmono">←</span> <em>or</em> shift h</dt>
          <dd>Scroll all the way left</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>shift <span class="badmono">→</span> <em>or</em> shift l</dt>
          <dd>Scroll all the way right</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>shift <span class="badmono">↑</span> <em>or</em> shift k</dt>
          <dd>Scroll all the way up</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>shift <span class="badmono">↓</span> <em>or</em> shift j</dt>
          <dd>Scroll all the way down</dd>
        </dl>
      </div><!-- /.column.last -->
    </div>
  </div>

  <div class="js-hidden-pane" >
    <div class="rule"></div>
    <div class="columns threecols">
      <div class="column first js-hidden-pane" >
        <h3>Source Code Browsing</h3>
        <dl class="keyboard-mappings">
          <dt>t</dt>
          <dd>Activates the file finder</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>l</dt>
          <dd>Jump to line</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>w</dt>
          <dd>Switch branch/tag</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>y</dt>
          <dd>Expand URL to its canonical form</dd>
        </dl>
      </div>
    </div>
  </div>

  <div class="js-hidden-pane" style='display:none'>
    <div class="rule"></div>
    <div class="columns threecols">
      <div class="column first">
        <h3>Browsing Commits</h3>
        <dl class="keyboard-mappings">
          <dt><span class="platform-mac">⌘</span><span class="platform-other">ctrl</span> <em>+</em> enter</dt>
          <dd>Submit comment</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>escape</dt>
          <dd>Close form</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>p</dt>
          <dd>Parent commit</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>o</dt>
          <dd>Other parent commit</dd>
        </dl>
      </div>
    </div>
  </div>

  <div class="js-hidden-pane" style='display:none'>
    <div class="rule"></div>
    <h3>Notifications</h3>

    <div class="columns threecols">
      <div class="column first">
        <dl class="keyboard-mappings">
          <dt>j</dt>
          <dd>Move selection down</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>k</dt>
          <dd>Move selection up</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>o <em>or</em> enter</dt>
          <dd>Open notification</dd>
        </dl>
      </div><!-- /.column.first -->

      <div class="column second">
        <dl class="keyboard-mappings">
          <dt>e <em>or</em> shift i <em>or</em> y</dt>
          <dd>Mark as read</dd>
        </dl>
        <dl class="keyboard-mappings">
          <dt>shift m</dt>
          <dd>Mute thread</dd>
        </dl>
      </div><!-- /.column.first -->
    </div>
  </div>

</div>

    <div id="markdown-help" class="instapaper_ignore readability-extra">
  <h2>Markdown Cheat Sheet</h2>

  <div class="cheatsheet-content">

  <div class="mod">
    <div class="col">
      <h3>Format Text</h3>
      <p>Headers</p>
      <pre>
# This is an &lt;h1&gt; tag
## This is an &lt;h2&gt; tag
###### This is an &lt;h6&gt; tag</pre>
     <p>Text styles</p>
     <pre>
*This text will be italic*
_This will also be italic_
**This text will be bold**
__This will also be bold__

*You **can** combine them*
</pre>
    </div>
    <div class="col">
      <h3>Lists</h3>
      <p>Unordered</p>
      <pre>
* Item 1
* Item 2
  * Item 2a
  * Item 2b</pre>
     <p>Ordered</p>
     <pre>
1. Item 1
2. Item 2
3. Item 3
   * Item 3a
   * Item 3b</pre>
    </div>
    <div class="col">
      <h3>Miscellaneous</h3>
      <p>Images</p>
      <pre>
![GitHub Logo](/images/logo.png)
Format: ![Alt Text](url)
</pre>
     <p>Links</p>
     <pre>
http://github.com - automatic!
[GitHub](http://github.com)</pre>
<p>Blockquotes</p>
     <pre>
As Kanye West said:

> We're living the future so
> the present is our past.
</pre>
    </div>
  </div>
  <div class="rule"></div>

  <h3>Code Examples in Markdown</h3>
  <div class="col">
      <p>Syntax highlighting with <a href="http://github.github.com/github-flavored-markdown/" title="GitHub Flavored Markdown" target="_blank">GFM</a></p>
      <pre>
```javascript
function fancyAlert(arg) {
  if(arg) {
    $.facebox({div:'#foo'})
  }
}
```</pre>
    </div>
    <div class="col">
      <p>Or, indent your code 4 spaces</p>
      <pre>
Here is a Python code example
without syntax highlighting:

    def foo:
      if not bar:
        return true</pre>
    </div>
    <div class="col">
      <p>Inline code for comments</p>
      <pre>
I think you should use an
`&lt;addr&gt;` element here instead.</pre>
    </div>
  </div>

  </div>
</div>


    <div id="ajax-error-message" class="flash flash-error">
      <span class="mini-icon mini-icon-exclamation"></span>
      Something went wrong with that request. Please try again.
      <a href="#" class="mini-icon mini-icon-remove-close ajax-error-dismiss"></a>
    </div>

    
    
    <span id='server_response_time' data-time='0.09070' data-host='fe18'></span>
    
  </body>
</html>

