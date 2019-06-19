<link href="/zimec/public/temas/simec/fonts/awesome/font-awesome.css?t=1.0" rel="stylesheet">

<style>
.acesso-rapido-theme-config {
    position:absolute;
    right: 0px;
    overflow: hidden;
    top: 140px
}
.acesso-rapido-theme-config-box {
    margin-right: -220px;
    postiton: relative;
    z-index: 2000;
    transition-duration: 0.8s;
}
.acesso-rapido-theme-config-box.show {
  margin-right: 0px;
}
.acesso-rapido-spin-icon {
  background: #1ab394;
  position: absolute;
  padding: 10px 10px 7px 13px;
  border-radius: 20px 0px 0px 20px;
  font-size: 16px;
  top: 0;
  left: 0px;
  width: 18px;
  height: 17px;
  color: #fff;
  cursor: pointer;
}
.acesso-rapido-skin-setttings {
  width: 220px;
  margin-left: 40px;
  background: #f3f3f4;
}
.acesso-rapido-skin-setttings .title {
  background: #efefef;
  text-align: center;
  text-transform: uppercase;
  font-weight: 600;
  display: block;
  padding: 10px 15px;
  font-size: 12px;
}
.acesso-rapido-setings-item {
  padding: 10px 30px;
}
.acesso-rapido-setings-item.skin {
  text-align: center;
}
.acesso-rapido-setings-item .switch {
  float: right;
}
.acesso-rapido-skin-name a {
  text-transform: uppercase;
}
.acesso-rapido-setings-item a {
  color: #fff;
}
.default-skin,
.blue-skin,
.ultra-skin,
.yellow-skin {
  text-align: center;
}
.default-skin {
  font-weight: 600;
  background: #1ab394;
}
.default-skin:hover {
  background: #199d82;
}
.blue-skin {
  font-weight: 600;
  background: #0d8ddb;
}
.blue-skin:hover {
  background: #0b82c6;
}
.yellow-skin {
  font-weight: 600;
  background: #ce8735;
}
.yellow-skin:hover {
  background: #b7782f;
}
.ultra-skin {
  font-weight: 600;
  background: #1a2d40;
}
.ultra-skin:hover {
  background: #111e2b;
}

.ui-tabs-nav li.ui-tabs-close-button {
    float: right !important;
    margin-top: 2px !important;
}
.ui-tabs-nav li.ui-tabs-close-button button{
    background: #FFFFFF;
    color: #7F7F7F;
    border: 1px solid #7F7F7F;
}

#acesso-rapido-tab .ui-widget-header {
    background: #FFFFFF;
    border: 0px;
}

#acesso-rapido-tab .ui-state-active {
    background: #FFFFFF;
    border: 0px;
    border-left: 1px solid #E7EAEC;
    border-right: 1px solid #E7EAEC;
}

#acesso-rapido-tab .ui-state-default a, .ui-state-default a:link {
    color: #777777;
    font-weight: bold;
}
#acesso-rapido-tab .ui-state-hover {
    background: #F5F5F5;
}
#acesso-rapido-tab .ui-state-hover a {
    color: #000000;    
}
#acesso-rapido-tab .ui-state-active a {
    color: #000000;
    font-weight: bold;
    text-decoration: underline;
}

.ui-widget input, .ui-widget select, .ui-widget textarea, .ui-widget button {
    font-family: "Helvetica Neue",Helvetica,Arial,sans-serif , Arial, Verdana !important;
    font-size: 14px !important;
}
</style>

<div class="acesso-rapido-theme-config" style="display:;">
	<div class="acesso-rapido-theme-config-box">
    	<div class="acesso-rapido-spin-icon">
        	<i class="fa fa-cogs fa-spin"></i>
		</div>
        <div class="acesso-rapido-skin-setttings">
        	<div class="title">
        		<i class="fa fa-arrow-circle-right"></i> Acesso rápido
        	</div>
		<?php 
        foreach ($dados['menus'] as $i => $d) {
        ?>              
			<div id-menu="<?php echo $d['acrid']; ?>" class="id-menu-acesso-rapido acesso-rapido-setings-item <?php echo $dados['arClassItem'][$i]; ?>">
                <span class="acesso-rapido-skin-name ">
                    <a href="#" class="s-skin-0"><?php echo $d['acrdsc']; ?></a>
            	</span>
			</div>
		<?php
    		$tabs .= '<li id-seletor-tab-acesso-rapido="'. $d['acrid'] .'" class="seletorAcessoRapido"><a href="#tabAcessoRapido-'. $d['acrid'] .'" onclick="buscarConteudoFiltroAcessoRapido('. $d['acrid'] .')">'. $d['acrdsc'] .'</a></li>';
    		$tabsConteudo .= '<div id-conteudo-tab-acesso-rapido="'. $d['acrid'] .'" id="tabAcessoRapido-'. $d['acrid'] .'" class="conteudoAcessoRapido"></div>';
        }
        ?>              
		</div>
	</div>
</div>

<div id="modalFiltroAcessoRapido" style="display: none;">
    <div id="acesso-rapido-tab">
        <ul>
        	<?php echo $tabs; ?>
        	<li class="ui-tabs-close-button"><button id="acesso-rapido-btnclose">X</button></li>
        </ul>
        <?php echo $tabsConteudo; ?>
    </div>
</div>
