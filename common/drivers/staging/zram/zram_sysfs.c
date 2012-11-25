  


<!DOCTYPE html>
<html>
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# githubog: http://ogp.me/ns/fb/githubog#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title>Kernel-Nexus7/drivers/staging/zram/zram_sysfs.c at master · motley-git/Kernel-Nexus7</title>
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
    

      <link rel='permalink' href='/motley-git/Kernel-Nexus7/blob/02eea732fc1f834d620eba4a5a21785fd913ab71/drivers/staging/zram/zram_sysfs.c'>
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
                      <a href="/motley-git/Kernel-Nexus7/blob/master/drivers/staging/zram/zram_sysfs.c" class="js-navigation-open" data-name="master" rel="nofollow">master</a>
                  </h4>
                </div>
                <div class="commitish-item branch-commitish selector-item js-navigation-item js-navigation-target ">
                  <span class="mini-icon mini-icon-confirm"></span>
                  <h4>
                      <a href="/motley-git/Kernel-Nexus7/blob/motley_v2/drivers/staging/zram/zram_sysfs.c" class="js-navigation-open" data-name="motley_v2" rel="nofollow">motley_v2</a>
                  </h4>
                </div>
                <div class="commitish-item branch-commitish selector-item js-navigation-item js-navigation-target ">
                  <span class="mini-icon mini-icon-confirm"></span>
                  <h4>
                      <a href="/motley-git/Kernel-Nexus7/blob/motley_v3/drivers/staging/zram/zram_sysfs.c" class="js-navigation-open" data-name="motley_v3" rel="nofollow">motley_v3</a>
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
          


<!-- blob contrib key: blob_contributors:v21:0c74f85fbd0d25548496d79830c04a1e -->
<!-- blob contrib frag key: views10/v8/blob_contributors:v21:0c74f85fbd0d25548496d79830c04a1e -->

<div id="slider">


    <p title="This is a placeholder element" class="js-history-link-replace hidden"></p>
    <div class="breadcrumb" data-path="drivers/staging/zram/zram_sysfs.c/">
      <span class='bold'><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/motley-git/Kernel-Nexus7" class="js-slider-breadcrumb" itemscope="url"><span itemprop="title">Kernel-Nexus7</span></a></span></span> / <span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/motley-git/Kernel-Nexus7/tree/master/drivers" class="js-slider-breadcrumb" itemscope="url"><span itemprop="title">drivers</span></a></span> / <span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/motley-git/Kernel-Nexus7/tree/master/drivers/staging" class="js-slider-breadcrumb" itemscope="url"><span itemprop="title">staging</span></a></span> / <span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/motley-git/Kernel-Nexus7/tree/master/drivers/staging/zram" class="js-slider-breadcrumb" itemscope="url"><span itemprop="title">zram</span></a></span> / <strong class="final-path">zram_sysfs.c</strong> <span class="js-clippy mini-icon mini-icon-clippy " data-clipboard-text="drivers/staging/zram/zram_sysfs.c" data-copied-hint="copied!" data-copy-hint="copy to clipboard"></span>
    </div>

    <a href="/motley-git/Kernel-Nexus7/find/master" class="js-slide-to" data-hotkey="t" style="display:none">Show File Finder</a>

      <div class="commit commit-loader file-history-tease js-deferred-content" data-url="/motley-git/Kernel-Nexus7/contributors/master/drivers/staging/zram/zram_sysfs.c" data-path="drivers/staging/zram/zram_sysfs.c/">
        Fetching contributors…

        <div class="participation">
          <p class="loader-loading"><img alt="Octocat-spinner-32-eaf2f5" height="16" src="https://a248.e.akamai.net/assets.github.com/images/spinners/octocat-spinner-32-EAF2F5.gif?1340659561" width="16" /></p>
          <p class="loader-error">Cannot retrieve contributors at this time</p>
        </div>
      </div>

    <div class="frames">
      <div class="frame frame-center" data-path="drivers/staging/zram/zram_sysfs.c/" data-permalink-url="/motley-git/Kernel-Nexus7/blob/02eea732fc1f834d620eba4a5a21785fd913ab71/drivers/staging/zram/zram_sysfs.c" data-title="Kernel-Nexus7/drivers/staging/zram/zram_sysfs.c at master · motley-git/Kernel-Nexus7 · GitHub" data-type="blob">

        <div id="files" class="bubble">
          <div class="file">
            <div class="meta">
              <div class="info">
                <span class="icon"><b class="mini-icon mini-icon-text-file"></b></span>
                <span class="mode" title="File Mode">file</span>
                  <span>222 lines (176 sloc)</span>
                <span>5.35 kb</span>
              </div>
              <ul class="button-group actions">
                  <li>
                        <a class="grouped-button minibutton bigger lighter tooltipped leftwards"
                           title="Clicking this button will automatically fork this project so you can edit the file"
                           href="/motley-git/Kernel-Nexus7/edit/master/drivers/staging/zram/zram_sysfs.c"
                           data-method="post" rel="nofollow">Edit</a>
                  </li>
                <li><a href="/motley-git/Kernel-Nexus7/raw/master/drivers/staging/zram/zram_sysfs.c" class="minibutton grouped-button bigger lighter" id="raw-url">Raw</a></li>
                  <li><a href="/motley-git/Kernel-Nexus7/blame/master/drivers/staging/zram/zram_sysfs.c" class="minibutton grouped-button bigger lighter">Blame</a></li>
                <li><a href="/motley-git/Kernel-Nexus7/commits/master/drivers/staging/zram/zram_sysfs.c" class="minibutton grouped-button bigger lighter" rel="nofollow">History</a></li>
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
</pre>
          </td>
          <td width="100%">
                <div class="highlight"><pre><div class='line' id='LC1'><span class="cm">/*</span></div><div class='line' id='LC2'><span class="cm"> * Compressed RAM block device</span></div><div class='line' id='LC3'><span class="cm"> *</span></div><div class='line' id='LC4'><span class="cm"> * Copyright (C) 2008, 2009, 2010  Nitin Gupta</span></div><div class='line' id='LC5'><span class="cm"> *</span></div><div class='line' id='LC6'><span class="cm"> * This code is released using a dual license strategy: BSD/GPL</span></div><div class='line' id='LC7'><span class="cm"> * You can choose the licence that better fits your requirements.</span></div><div class='line' id='LC8'><span class="cm"> *</span></div><div class='line' id='LC9'><span class="cm"> * Released under the terms of 3-clause BSD License</span></div><div class='line' id='LC10'><span class="cm"> * Released under the terms of GNU General Public License Version 2.0</span></div><div class='line' id='LC11'><span class="cm"> *</span></div><div class='line' id='LC12'><span class="cm"> * Project home: http://compcache.googlecode.com/</span></div><div class='line' id='LC13'><span class="cm"> */</span></div><div class='line' id='LC14'><br/></div><div class='line' id='LC15'><span class="cp">#include &lt;linux/device.h&gt;</span></div><div class='line' id='LC16'><span class="cp">#include &lt;linux/genhd.h&gt;</span></div><div class='line' id='LC17'><span class="cp">#include &lt;linux/mm.h&gt;</span></div><div class='line' id='LC18'><br/></div><div class='line' id='LC19'><span class="cp">#include &quot;zram_drv.h&quot;</span></div><div class='line' id='LC20'><br/></div><div class='line' id='LC21'><span class="k">static</span> <span class="n">u64</span> <span class="nf">zram_stat64_read</span><span class="p">(</span><span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span><span class="p">,</span> <span class="n">u64</span> <span class="o">*</span><span class="n">v</span><span class="p">)</span></div><div class='line' id='LC22'><span class="p">{</span></div><div class='line' id='LC23'>	<span class="n">u64</span> <span class="n">val</span><span class="p">;</span></div><div class='line' id='LC24'><br/></div><div class='line' id='LC25'>	<span class="n">spin_lock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">stat64_lock</span><span class="p">);</span></div><div class='line' id='LC26'>	<span class="n">val</span> <span class="o">=</span> <span class="o">*</span><span class="n">v</span><span class="p">;</span></div><div class='line' id='LC27'>	<span class="n">spin_unlock</span><span class="p">(</span><span class="o">&amp;</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">stat64_lock</span><span class="p">);</span></div><div class='line' id='LC28'><br/></div><div class='line' id='LC29'>	<span class="k">return</span> <span class="n">val</span><span class="p">;</span></div><div class='line' id='LC30'><span class="p">}</span></div><div class='line' id='LC31'><br/></div><div class='line' id='LC32'><span class="k">static</span> <span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="nf">dev_to_zram</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">)</span></div><div class='line' id='LC33'><span class="p">{</span></div><div class='line' id='LC34'>	<span class="kt">int</span> <span class="n">i</span><span class="p">;</span></div><div class='line' id='LC35'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC36'><br/></div><div class='line' id='LC37'>	<span class="k">for</span> <span class="p">(</span><span class="n">i</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">i</span> <span class="o">&lt;</span> <span class="n">num_devices</span><span class="p">;</span> <span class="n">i</span><span class="o">++</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC38'>		<span class="n">zram</span> <span class="o">=</span> <span class="o">&amp;</span><span class="n">devices</span><span class="p">[</span><span class="n">i</span><span class="p">];</span></div><div class='line' id='LC39'>		<span class="k">if</span> <span class="p">(</span><span class="n">disk_to_dev</span><span class="p">(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">disk</span><span class="p">)</span> <span class="o">==</span> <span class="n">dev</span><span class="p">)</span></div><div class='line' id='LC40'>			<span class="k">break</span><span class="p">;</span></div><div class='line' id='LC41'>	<span class="p">}</span></div><div class='line' id='LC42'><br/></div><div class='line' id='LC43'>	<span class="k">return</span> <span class="n">zram</span><span class="p">;</span></div><div class='line' id='LC44'><span class="p">}</span></div><div class='line' id='LC45'><br/></div><div class='line' id='LC46'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">disksize_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC47'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC48'><span class="p">{</span></div><div class='line' id='LC49'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC50'><br/></div><div class='line' id='LC51'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%llu</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span> <span class="n">zram</span><span class="o">-&gt;</span><span class="n">disksize</span><span class="p">);</span></div><div class='line' id='LC52'><span class="p">}</span></div><div class='line' id='LC53'><br/></div><div class='line' id='LC54'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">disksize_store</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC55'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="k">const</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">,</span> <span class="kt">size_t</span> <span class="n">len</span><span class="p">)</span></div><div class='line' id='LC56'><span class="p">{</span></div><div class='line' id='LC57'>	<span class="kt">int</span> <span class="n">ret</span><span class="p">;</span></div><div class='line' id='LC58'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC59'><br/></div><div class='line' id='LC60'>	<span class="k">if</span> <span class="p">(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">init_done</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC61'>		<span class="n">pr_info</span><span class="p">(</span><span class="s">&quot;Cannot change disksize for initialized device</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">);</span></div><div class='line' id='LC62'>		<span class="k">return</span> <span class="o">-</span><span class="n">EBUSY</span><span class="p">;</span></div><div class='line' id='LC63'>	<span class="p">}</span></div><div class='line' id='LC64'><br/></div><div class='line' id='LC65'>	<span class="n">ret</span> <span class="o">=</span> <span class="n">strict_strtoull</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="mi">10</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">disksize</span><span class="p">);</span></div><div class='line' id='LC66'>	<span class="k">if</span> <span class="p">(</span><span class="n">ret</span><span class="p">)</span></div><div class='line' id='LC67'>		<span class="k">return</span> <span class="n">ret</span><span class="p">;</span></div><div class='line' id='LC68'><br/></div><div class='line' id='LC69'>	<span class="n">zram</span><span class="o">-&gt;</span><span class="n">disksize</span> <span class="o">=</span> <span class="n">PAGE_ALIGN</span><span class="p">(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">disksize</span><span class="p">);</span></div><div class='line' id='LC70'>	<span class="n">set_capacity</span><span class="p">(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">disk</span><span class="p">,</span> <span class="n">zram</span><span class="o">-&gt;</span><span class="n">disksize</span> <span class="o">&gt;&gt;</span> <span class="n">SECTOR_SHIFT</span><span class="p">);</span></div><div class='line' id='LC71'><br/></div><div class='line' id='LC72'>	<span class="k">return</span> <span class="n">len</span><span class="p">;</span></div><div class='line' id='LC73'><span class="p">}</span></div><div class='line' id='LC74'><br/></div><div class='line' id='LC75'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">initstate_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC76'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC77'><span class="p">{</span></div><div class='line' id='LC78'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC79'><br/></div><div class='line' id='LC80'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%u</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span> <span class="n">zram</span><span class="o">-&gt;</span><span class="n">init_done</span><span class="p">);</span></div><div class='line' id='LC81'><span class="p">}</span></div><div class='line' id='LC82'><br/></div><div class='line' id='LC83'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">reset_store</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC84'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="k">const</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">,</span> <span class="kt">size_t</span> <span class="n">len</span><span class="p">)</span></div><div class='line' id='LC85'><span class="p">{</span></div><div class='line' id='LC86'>	<span class="kt">int</span> <span class="n">ret</span><span class="p">;</span></div><div class='line' id='LC87'>	<span class="kt">unsigned</span> <span class="kt">long</span> <span class="n">do_reset</span><span class="p">;</span></div><div class='line' id='LC88'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span><span class="p">;</span></div><div class='line' id='LC89'>	<span class="k">struct</span> <span class="n">block_device</span> <span class="o">*</span><span class="n">bdev</span><span class="p">;</span></div><div class='line' id='LC90'><br/></div><div class='line' id='LC91'>	<span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC92'>	<span class="n">bdev</span> <span class="o">=</span> <span class="n">bdget_disk</span><span class="p">(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">disk</span><span class="p">,</span> <span class="mi">0</span><span class="p">);</span></div><div class='line' id='LC93'><br/></div><div class='line' id='LC94'>	<span class="cm">/* Do not reset an active device! */</span></div><div class='line' id='LC95'>	<span class="k">if</span> <span class="p">(</span><span class="n">bdev</span><span class="o">-&gt;</span><span class="n">bd_holders</span><span class="p">)</span></div><div class='line' id='LC96'>		<span class="k">return</span> <span class="o">-</span><span class="n">EBUSY</span><span class="p">;</span></div><div class='line' id='LC97'><br/></div><div class='line' id='LC98'>	<span class="n">ret</span> <span class="o">=</span> <span class="n">strict_strtoul</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="mi">10</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">do_reset</span><span class="p">);</span></div><div class='line' id='LC99'>	<span class="k">if</span> <span class="p">(</span><span class="n">ret</span><span class="p">)</span></div><div class='line' id='LC100'>		<span class="k">return</span> <span class="n">ret</span><span class="p">;</span></div><div class='line' id='LC101'><br/></div><div class='line' id='LC102'>	<span class="k">if</span> <span class="p">(</span><span class="o">!</span><span class="n">do_reset</span><span class="p">)</span></div><div class='line' id='LC103'>		<span class="k">return</span> <span class="o">-</span><span class="n">EINVAL</span><span class="p">;</span></div><div class='line' id='LC104'><br/></div><div class='line' id='LC105'>	<span class="cm">/* Make sure all pending I/O is finished */</span></div><div class='line' id='LC106'>	<span class="k">if</span> <span class="p">(</span><span class="n">bdev</span><span class="p">)</span></div><div class='line' id='LC107'>		<span class="n">fsync_bdev</span><span class="p">(</span><span class="n">bdev</span><span class="p">);</span></div><div class='line' id='LC108'><br/></div><div class='line' id='LC109'>	<span class="k">if</span> <span class="p">(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">init_done</span><span class="p">)</span></div><div class='line' id='LC110'>		<span class="n">zram_reset_device</span><span class="p">(</span><span class="n">zram</span><span class="p">);</span></div><div class='line' id='LC111'><br/></div><div class='line' id='LC112'>	<span class="k">return</span> <span class="n">len</span><span class="p">;</span></div><div class='line' id='LC113'><span class="p">}</span></div><div class='line' id='LC114'><br/></div><div class='line' id='LC115'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">num_reads_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC116'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC117'><span class="p">{</span></div><div class='line' id='LC118'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC119'><br/></div><div class='line' id='LC120'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%llu</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span></div><div class='line' id='LC121'>		<span class="n">zram_stat64_read</span><span class="p">(</span><span class="n">zram</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">stats</span><span class="p">.</span><span class="n">num_reads</span><span class="p">));</span></div><div class='line' id='LC122'><span class="p">}</span></div><div class='line' id='LC123'><br/></div><div class='line' id='LC124'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">num_writes_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC125'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC126'><span class="p">{</span></div><div class='line' id='LC127'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC128'><br/></div><div class='line' id='LC129'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%llu</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span></div><div class='line' id='LC130'>		<span class="n">zram_stat64_read</span><span class="p">(</span><span class="n">zram</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">stats</span><span class="p">.</span><span class="n">num_writes</span><span class="p">));</span></div><div class='line' id='LC131'><span class="p">}</span></div><div class='line' id='LC132'><br/></div><div class='line' id='LC133'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">invalid_io_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC134'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC135'><span class="p">{</span></div><div class='line' id='LC136'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC137'><br/></div><div class='line' id='LC138'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%llu</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span></div><div class='line' id='LC139'>		<span class="n">zram_stat64_read</span><span class="p">(</span><span class="n">zram</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">stats</span><span class="p">.</span><span class="n">invalid_io</span><span class="p">));</span></div><div class='line' id='LC140'><span class="p">}</span></div><div class='line' id='LC141'><br/></div><div class='line' id='LC142'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">notify_free_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC143'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC144'><span class="p">{</span></div><div class='line' id='LC145'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC146'><br/></div><div class='line' id='LC147'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%llu</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span></div><div class='line' id='LC148'>		<span class="n">zram_stat64_read</span><span class="p">(</span><span class="n">zram</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">stats</span><span class="p">.</span><span class="n">notify_free</span><span class="p">));</span></div><div class='line' id='LC149'><span class="p">}</span></div><div class='line' id='LC150'><br/></div><div class='line' id='LC151'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">zero_pages_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC152'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC153'><span class="p">{</span></div><div class='line' id='LC154'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC155'><br/></div><div class='line' id='LC156'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%u</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span> <span class="n">zram</span><span class="o">-&gt;</span><span class="n">stats</span><span class="p">.</span><span class="n">pages_zero</span><span class="p">);</span></div><div class='line' id='LC157'><span class="p">}</span></div><div class='line' id='LC158'><br/></div><div class='line' id='LC159'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">orig_data_size_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC160'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC161'><span class="p">{</span></div><div class='line' id='LC162'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC163'><br/></div><div class='line' id='LC164'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%llu</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span></div><div class='line' id='LC165'>		<span class="p">(</span><span class="n">u64</span><span class="p">)(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">stats</span><span class="p">.</span><span class="n">pages_stored</span><span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="n">PAGE_SHIFT</span><span class="p">);</span></div><div class='line' id='LC166'><span class="p">}</span></div><div class='line' id='LC167'><br/></div><div class='line' id='LC168'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">compr_data_size_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC169'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC170'><span class="p">{</span></div><div class='line' id='LC171'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC172'><br/></div><div class='line' id='LC173'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%llu</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span></div><div class='line' id='LC174'>		<span class="n">zram_stat64_read</span><span class="p">(</span><span class="n">zram</span><span class="p">,</span> <span class="o">&amp;</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">stats</span><span class="p">.</span><span class="n">compr_size</span><span class="p">));</span></div><div class='line' id='LC175'><span class="p">}</span></div><div class='line' id='LC176'><br/></div><div class='line' id='LC177'><span class="k">static</span> <span class="kt">ssize_t</span> <span class="nf">mem_used_total_show</span><span class="p">(</span><span class="k">struct</span> <span class="n">device</span> <span class="o">*</span><span class="n">dev</span><span class="p">,</span></div><div class='line' id='LC178'>		<span class="k">struct</span> <span class="n">device_attribute</span> <span class="o">*</span><span class="n">attr</span><span class="p">,</span> <span class="kt">char</span> <span class="o">*</span><span class="n">buf</span><span class="p">)</span></div><div class='line' id='LC179'><span class="p">{</span></div><div class='line' id='LC180'>	<span class="n">u64</span> <span class="n">val</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC181'>	<span class="k">struct</span> <span class="n">zram</span> <span class="o">*</span><span class="n">zram</span> <span class="o">=</span> <span class="n">dev_to_zram</span><span class="p">(</span><span class="n">dev</span><span class="p">);</span></div><div class='line' id='LC182'><br/></div><div class='line' id='LC183'>	<span class="k">if</span> <span class="p">(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">init_done</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC184'>		<span class="n">val</span> <span class="o">=</span> <span class="n">xv_get_total_size_bytes</span><span class="p">(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">mem_pool</span><span class="p">)</span> <span class="o">+</span></div><div class='line' id='LC185'>			<span class="p">((</span><span class="n">u64</span><span class="p">)(</span><span class="n">zram</span><span class="o">-&gt;</span><span class="n">stats</span><span class="p">.</span><span class="n">pages_expand</span><span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="n">PAGE_SHIFT</span><span class="p">);</span></div><div class='line' id='LC186'>	<span class="p">}</span></div><div class='line' id='LC187'><br/></div><div class='line' id='LC188'>	<span class="k">return</span> <span class="n">sprintf</span><span class="p">(</span><span class="n">buf</span><span class="p">,</span> <span class="s">&quot;%llu</span><span class="se">\n</span><span class="s">&quot;</span><span class="p">,</span> <span class="n">val</span><span class="p">);</span></div><div class='line' id='LC189'><span class="p">}</span></div><div class='line' id='LC190'><br/></div><div class='line' id='LC191'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">disksize</span><span class="p">,</span> <span class="n">S_IRUGO</span> <span class="o">|</span> <span class="n">S_IWUSR</span><span class="p">,</span></div><div class='line' id='LC192'>		<span class="n">disksize_show</span><span class="p">,</span> <span class="n">disksize_store</span><span class="p">);</span></div><div class='line' id='LC193'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">initstate</span><span class="p">,</span> <span class="n">S_IRUGO</span><span class="p">,</span> <span class="n">initstate_show</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC194'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">reset</span><span class="p">,</span> <span class="n">S_IWUSR</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">,</span> <span class="n">reset_store</span><span class="p">);</span></div><div class='line' id='LC195'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">num_reads</span><span class="p">,</span> <span class="n">S_IRUGO</span><span class="p">,</span> <span class="n">num_reads_show</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC196'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">num_writes</span><span class="p">,</span> <span class="n">S_IRUGO</span><span class="p">,</span> <span class="n">num_writes_show</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC197'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">invalid_io</span><span class="p">,</span> <span class="n">S_IRUGO</span><span class="p">,</span> <span class="n">invalid_io_show</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC198'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">notify_free</span><span class="p">,</span> <span class="n">S_IRUGO</span><span class="p">,</span> <span class="n">notify_free_show</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC199'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">zero_pages</span><span class="p">,</span> <span class="n">S_IRUGO</span><span class="p">,</span> <span class="n">zero_pages_show</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC200'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">orig_data_size</span><span class="p">,</span> <span class="n">S_IRUGO</span><span class="p">,</span> <span class="n">orig_data_size_show</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC201'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">compr_data_size</span><span class="p">,</span> <span class="n">S_IRUGO</span><span class="p">,</span> <span class="n">compr_data_size_show</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC202'><span class="k">static</span> <span class="n">DEVICE_ATTR</span><span class="p">(</span><span class="n">mem_used_total</span><span class="p">,</span> <span class="n">S_IRUGO</span><span class="p">,</span> <span class="n">mem_used_total_show</span><span class="p">,</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC203'><br/></div><div class='line' id='LC204'><span class="k">static</span> <span class="k">struct</span> <span class="n">attribute</span> <span class="o">*</span><span class="n">zram_disk_attrs</span><span class="p">[]</span> <span class="o">=</span> <span class="p">{</span></div><div class='line' id='LC205'>	<span class="o">&amp;</span><span class="n">dev_attr_disksize</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC206'>	<span class="o">&amp;</span><span class="n">dev_attr_initstate</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC207'>	<span class="o">&amp;</span><span class="n">dev_attr_reset</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC208'>	<span class="o">&amp;</span><span class="n">dev_attr_num_reads</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC209'>	<span class="o">&amp;</span><span class="n">dev_attr_num_writes</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC210'>	<span class="o">&amp;</span><span class="n">dev_attr_invalid_io</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC211'>	<span class="o">&amp;</span><span class="n">dev_attr_notify_free</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC212'>	<span class="o">&amp;</span><span class="n">dev_attr_zero_pages</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC213'>	<span class="o">&amp;</span><span class="n">dev_attr_orig_data_size</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC214'>	<span class="o">&amp;</span><span class="n">dev_attr_compr_data_size</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC215'>	<span class="o">&amp;</span><span class="n">dev_attr_mem_used_total</span><span class="p">.</span><span class="n">attr</span><span class="p">,</span></div><div class='line' id='LC216'>	<span class="nb">NULL</span><span class="p">,</span></div><div class='line' id='LC217'><span class="p">};</span></div><div class='line' id='LC218'><br/></div><div class='line' id='LC219'><span class="k">struct</span> <span class="n">attribute_group</span> <span class="n">zram_disk_attr_group</span> <span class="o">=</span> <span class="p">{</span></div><div class='line' id='LC220'>	<span class="p">.</span><span class="n">attrs</span> <span class="o">=</span> <span class="n">zram_disk_attrs</span><span class="p">,</span></div><div class='line' id='LC221'><span class="p">};</span></div></pre></div>
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


    <p class="right">&copy; 2012 <span title="0.15004s from fe18.rs.github.com">GitHub</span> Inc. All rights reserved.</p>
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

    
    
    <span id='server_response_time' data-time='0.15125' data-host='fe18'></span>
    
  </body>
</html>

