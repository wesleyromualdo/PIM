<div class="theme-config" style="top: 65px; display:none;">
	<div class="theme-config-box">
    	<div class="spin-icon">
        	<i class="fa fa-cogs fa-spin"></i>
		</div>
        <div class="skin-setttings">
        	<div class="title">
        		<i class="fa fa-arrow-circle-right"></i> Acesso rápido
        	</div>
		<?php 
        foreach ($dados['menus'] as $i => $d){
        ?>              
			<div id-menu="<?php echo $d['acrid']; ?>" class="id-menu-acesso-rapido setings-item <?php echo $dados['arClassItem'][$i]; ?>">
                <span class="skin-name ">
                    <a href="#" class="s-skin-0"><?php echo $d['acrdsc']; ?></a>
            	</span>
			</div>
		<?php
		$tabs .= '<li id-seletor-tab-acesso-rapido="'. $d['acrid'] .'" class="seletorAcessoRapido"><a data-toggle="tab" href="#tabAcessoRapido-'. $d['acrid'] .'" onclick="buscarConteudoFiltroAcessoRapido('. $d['acrid'] .')">'. $d['acrdsc'] .'</a></li>';
		$tabsConteudo .= '<div id-conteudo-tab-acesso-rapido="'. $d['acrid'] .'" id="tabAcessoRapido-'. $d['acrid'] .'" class="tab-pane conteudoAcessoRapido"><div class="panel-body"></div></div>';
        }
        ?>              
		</div>
	</div>
</div>

<div class="ibox float-e-margins animated modal" id="modalFiltroAcessoRapido" tabdesex="-1" role="dialog" aria-hidden="true">
	<div class="modal-dialog modal-lg">
		<form method="post" name="acesso-rapido-form-filtro" id="acesso-rapido-form-filtro" class="form form-horizontal">
			<div class="modal-content">
    			<div class="tabs-container">
    				<button type="button" class="close" data-dismiss="modal" aria-label="Close" style="padding: 9px;">
    					<span aria-hidden="true">×</span>
    				</button>
    				<ul class="nav nav-tabs">
    					<?php echo $tabs; ?>
    				</ul>
    				<div class="tab-content">
    					<?php echo $tabsConteudo; ?>
    				</div>					
    			</div>
			</div>
		</form>
	</div>
</div>
