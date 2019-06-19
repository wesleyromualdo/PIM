<nav class="navbar-default navbar-static-side" role="navigation" style="z-index: 2030;">

    <div class="sidebar-collapse">
        <ul class="nav metismenu" id="side-menu">
<!--
            <li class="nav-header">
                <div class="logo-element">
                    SISFOR
                </div>
            </li>-->

            @if( is_array($menus) && count($menus) > 0 )

                @foreach ($menus as $nivel1)
                    @if(isset($nivel1['itensMenu']) && count($nivel1['itensMenu']) > 0 && $nivel1['mnushow'] == true)
                        <li class="{{ isActiveRoute('main') }}">
                            <a href="{{$nivel1['mnudsc']}}">
                                <i class="{{ !empty($nivel1['mnuimagem']) ? ($nivel1['mnuimagem']) : 'fa fa-th-large'  }}"></i>
<!--                                <i class="fa fa-th-large"></i>-->
                                <span class="nav-label">{{$nivel1['mnudsc']}}</span> <span class="fa arrow"></span>
                            </a>
                            <ul class="nav nav-second-level">

                                @if(isset($nivel1['itensMenu']) && count($nivel1['itensMenu']) > 0)
                                    @foreach ($nivel1['itensMenu'] as $nivel2)
                                        @if($nivel2['mnushow'] == true)
                                            @if(isset($nivel2['mnutipo']) && $nivel2['mnutipo'] == 2 && $nivel2['mnusnsubmenu'] == false)
                                                <li class="{{ isActiveRoute('main') }}">
                                                    <a href="{{ url($nivel2['menu_url']) }}">

                                                        <i class="{{ !empty($nivel2['mnuimagem']) ? ($nivel2['mnuimagem']) : 'fa fa-gears'  }}"></i> {{$nivel2['mnudsc']}}
                                                    </a>
                                                </li>
                                            @else
                                                <li class="{{ isActiveRoute('main') }}">
                                                    <a href="#'{{$nivel2['mnudsc']}}">
                                                       <i class="{{ !empty($nivel2['mnuimagem']) ? ($nivel2['mnuimagem']) : 'fa fa-bars'  }}"></i>  {{$nivel2['mnudsc']}}<span class="fa arrow"></span>
                                                    </a>
                                                    <ul class="nav nav-third-level">

                                                        @if(isset($nivel2['itensMenu']) && count($nivel2['itensMenu']) > 0)
                                                            @foreach ($nivel2['itensMenu'] as $nivel3)
                                                                @if($nivel3['mnushow'] == true)
                                                                    <li class="{{ isActiveRoute('main') }}">
                                                                        <a href="{{ url($nivel3['menu_url']) }}">
                                                                            <i class="{{ !empty($nivel3['mnuimagem']) ? ($nivel3['mnuimagem']) : 'fa fa-gears'  }}"></i> {{$nivel3['mnudsc']}}
                                                                        </a>
                                                                    </li>
                                                                @endif

                                                            @endforeach
                                                        @endif
                                                    </ul>
                                                </li>
                                            @endif
                                        @endif
                                    @endforeach
                                @elseif($nivel2['mnushow'] == true)
                                    <li class="{{ isActiveRoute('main') }}">
                                        <a href="{{ url($nivel2['menu_url']) }}">
                                            <i class="fa fa-ellipsis-v"></i> {{$nivel2['mnudsc']}}
                                        </a>
                                    </li>
                                @endif

                            </ul>
                        </li>
                    @elseif($nivel1['mnushow'] == true)

                        <li class="{{ isActiveRoute('main') }}">
                            
                            @if($nivel1['menu_url'])
                            <a href="{{ url($nivel1['menu_url']) }}">
                                <i class="{{ !empty($nivel1['mnuimagem']) ? ($nivel1['mnuimagem']) : 'fa fa-th-large'  }}"></i>
                                 <span class="nav-label">{{$nivel1['mnudsc']}}</span>
                            </a>
                            @endif
                        </li>

                    @endif
                @endforeach
            @endif
        </ul>
    </div>

</nav>
