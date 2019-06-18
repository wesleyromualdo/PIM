<?php
$perfis = pegaPerfilGeral($_SESSION['usucpf']);
// Necessário porque é ajax
foreach($_REQUEST as $k => $v)
{
	$_REQUEST[$k] = ($v);
}

// Recupera valores post
$inuid = $_REQUEST['inuid'];
$grupo = $_REQUEST['grupo'];
$prgid = $_REQUEST['prgid'];
$tipoPesquisa 	= $_REQUEST['tipopesquisa'];

$objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
$objEscolas = new Par3_Model_Escolasproemi2015();


$modelProadesao = new Par3_Model_Proadesao();
$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($prgid);


if($inuid)
{
	$objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
}
else
{
	die("erro");
}

$arrParamCarga['inuid'] = $inuid;
$arrParamCarga['prgid'] = $prgid;
$arrParamCarga['grupo'] = $grupo;
$objRespostaCargaHoraria	= new Par3_Model_Respostacargahorariaprograma();
$arrCargaHoraria = $objRespostaCargaHoraria->getResposta($arrParamCarga);

if( (! $arrCargaHoraria) || ( $_REQUEST['carga_horaria'] != '') )
{
	if(  $_REQUEST['carga_horaria'] != '' )
	{
		$chekedSim = ($_REQUEST['carga_horaria'] == 'S' ) ? 'checked=checked' : '';
		$chekedNao = ($_REQUEST['carga_horaria'] == 'N' ) ? 'checked=checked' : '';
		$cargaHoraria = $_REQUEST['carga_horaria'];
	}
	else 
	{
		$cargaHoraria = false;
	}
}
else 
{
	
	$cargaHoraria = $arrCargaHoraria['rchresposta'];
	$rchid			= $arrCargaHoraria['rchid'];
	if( is_array($arrCargaHoraria))
	{
		$resposta2 	= $arrCargaHoraria['rchresposta'];
		$rchid		= $arrCargaHoraria['rchid'];
		 
		if($resposta2)
		{
			$chekedSim = ($resposta2 == 'S' ) ? 'checked=checked' : '';
			$chekedNao = ($resposta2 == 'N' ) ? 'checked=checked' : '';
		}
	}
}

$dadosUnidade = $objIstrumentoUnidade->getDadosUnidade($inuid);


$itrid = $dadosUnidade['itrid'];
$estuf = $dadosUnidade['estuf'];


$arrParametros = array();
$arrayParams['carga_horaria']	= $cargaHoraria;
$arrayParams['rchid']			= $rchid;
$arrayParams['itrid']			= $itrid;
$arrayParams['estuf']			= $estuf;
$arrayParams['inuid']			= $inuid;
$arrayParams['prgid']			= $prgid;
$arrayParams['inudescricao']	= $dadosUnidade['inudescricao'];
$arrayParams['muncod']			= $dadosUnidade['muncod'];
$arrayParams['grupo']			= $grupo;
$arrayParams['epemunicipio']	= $_REQUEST['epemunicipio'];
$arrayParams['epecod']			= $_REQUEST['epecod'];
$arrayParams['epenome']			= $_REQUEST['epenome'];
$arrayParams['checado']			= $_REQUEST['checado'];
$arrayParams['aepcargahoraria']	= $_REQUEST['aepcargahoraria'];
$arrayParams['ordenacao']		= $_REQUEST['ordenacao'];



if($grupo == 'N')
{
	$listaInfoEscolas = $objEscolas->getListaEscolasNaoElegiveis($arrayParams);
	$bloqueiaSave = $listaInfoEscolas['bloqueio'];
	$cabecalho 	= $listaInfoEscolas['cabecalho'];
	$dados 		= $listaInfoEscolas['arrayEscolas'];
	
	if($tipoPesquisa == 'xls')
	{
		$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
		$listagem->setCabecalho($cabecalho);
		$listagem->setDados($dados);
		$listagem->turnOffForm();
		$listagem->turnOnOrdenacao();
		$listagem->render();
		die();
	}
	else
	{
		$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		$listagem->setCabecalho($cabecalho);
		$listagem->setDados($dados);
		$listagem->turnOffForm();
		$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS, "*");
		$listagem->turnOnOrdenacao();
	}
	
}
else if( $tipoPesquisa == 'xls') {
	$listaInfoEscolas = $objEscolas->getListaEscolasElegiveisExcel($arrayParams);
	$bloqueiaSave = $listaInfoEscolas['bloqueio'];
	$cabecalho 	= $listaInfoEscolas['cabecalho'];
	$dados 		= $listaInfoEscolas['arrayEscolas'];
	$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
	$listagem->setCabecalho($cabecalho);
	$listagem->setDados($dados);
	$listagem->turnOffForm();
	$listagem->turnOnOrdenacao();
	$listagem->render();
	die();
	
}
else{
	$listaInfoEscolas = $objEscolas->getListaEscolasElegiveis($arrayParams);
	
	$bloqueiaSave = $listaInfoEscolas['bloqueio'];
	
	if( $listaInfoEscolas['status'] == 'success')
	{
	
		
		$cabecalho 	= $listaInfoEscolas['cabecalho'];
		$dados 		= $listaInfoEscolas['arrayEscolas'];
		$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		$listagem->setCabecalho($cabecalho);
		$listagem->setDados($dados);
		$listagem->turnOffForm();
		$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
		$listagem->turnOnOrdenacao();
	
	}
}


?>
<script>
$(document).ready(function(){
 	$('.btn-xls').hide();
	$('.btn-query').hide();

	if( $("#tipoPesquisa2").val() == 'lista')
	{
		window.location.hash = '#resultado';
	}

	$('.ordenavel').click(function(){
		var campo = $(this).attr('data-campo-ordenacao');
    	$('#ordenacao').val(campo);
    	$('#filtrar_pesquisa').click()
    	
    });
	
});
</script>
<div class="ibox-content">
	<form method="post" name="formulario_escolas" id="formulario_escolas" class="form form-horizontal">
		<input type="hidden" value="salvar_vinculo_escola" id="requisicao" name="requisicao" />
		<input type="hidden" value="N" id="carga_horaria_form" name="carga_horaria_escolas" />
		<input type="hidden" value="<?php echo $grupo; ?>" id="grupo" name="grupo" />
		<input type="hidden" value="<?php echo $grupo; ?>" id="grupoPopUp" name="grupoPopUp" />
		<input type="hidden" value="<?php echo $_REQUEST['ordenacao']?>" id="ordenacao" name="ordenacao" />
		<input type="hidden" value="<?php echo $tipoPesquisa?>" id="tipoPesquisa2" name="tipoPesquisa2" />
		
		<label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1">
			Estado
		</label>
		<div class="col-sm-8 col-md-8 col-lg-8 " >
	       <?php 				
			$sql = Territorios_Model_Estado::pegarSQLSelect ( $_REQUEST );
	
			$arrAttr = array (
				'data-placeholder' => 'Estado',
				'disabled'		=> 'disabled'
			);
			echo $simec->select ( 'estuf', null, $estuf , $sql, $arrAttr );
			?>
		</div>
		<label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1">
			Município
		</label>
		<div class="col-sm-8 col-md-8 col-lg-8 " >
		
		<?php 				
			$mundescricao = simec_htmlentities ( $_REQUEST ['mundescricao'] );
			// Caso estadual
			$arrAttr = array (
					'placeHolder' => 'Municipio',
					'maxlength' => '100',
					'id'		=> 'epemunicipio'
			);
			$default = $_REQUEST['epemunicipio'];
			
			echo $simec->input ( 'epemunicipio', null, $default, $arrAttr );
	 		?>
 		</div>
 		
 		<label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1">
			Dependência Administrativa
		</label>
		<div class="col-sm-8 col-md-8 col-lg-8 " >
	       <?php 				
			$arrOp = array(
				'Estadual' => 'Estadual',
				'Municipal' => 'Municipal'
			);
	
			if($itrid == 1 || $itrid == 3)
			{
				$rede = 'Estadual';
			}
			else {
				$rede = 'Municipal';
			}
			
			$arrAttr = array (
				'data-placeholder' => 'Dependência Administrativa',
				'id' => 'eperede',
				'disabled' => 'disabled'
			);
			
			echo $simec->select ( 'eperede', null, $rede , $arrOp, $arrAttr );
			?>
		</div>
		
		<label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1">
			Código INEP
		</label>
		<div class="col-sm-8 col-md-8 col-lg-8 " >
		<?php 				
			
			$arrAttr = array (
					'placeHolder' => 'Código INEP',
					'maxlength' => '12',
					'id'		=> 'epecod'
			);
		
			echo $simec->input( 'epecod', null, $_REQUEST['epecod'], $arrAttr );
	 		?>
 		</div>
 		
		<label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1">
			Nome da Escola
		</label>
		<div class="col-sm-8 col-md-8 col-lg-8 " >
		<?php 				
			
			$arrAttr = array (
					'placeHolder' => 'Nome da Escola',
					'maxlength' => '200',
					'epenome'		=> 'epenome'
			);
		
			echo $simec->input( 'epenome', null, $_REQUEST['epenome'], $arrAttr );
	 		?>
 		</div>
		
		<?php 
		if( $grupo != 'N')
		{
		?>
			<label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1">
				Selecionadas
			</label>
			<div class="col-sm-8 col-md-8 col-lg-8 " >
			<?php 				
				
				$arrOp = array(
					'' => '',
					'S' => 'Sim',
					'N' => 'Não'
				);
		
				$arrAttr = array (
					'data-placeholder' => 'Selecionadas',
					'id' => 'checado'
				);
				
				echo $simec->select ( 'checado', null, $_REQUEST['checado'] , $arrOp, $arrAttr );
		 		?>
	 		</div>
	 		<label class="col-sm-4 col-md-4 col-lg-4 control-label" for="resp1">
				Carga Horária
			</label>
			<div class="col-sm-8 col-md-8 col-lg-8 " >
			<?php 				
				
				$arrOp = array(
					'' => '',
					'5' => '5',
					'7' => '7'
				);
						
				$arrAttr = array (
					'data-placeholder' => 'Carga Horária',
					'id' => 'aepcargahoraria'
				);
				
				echo $simec->select ( 'aepcargahoraria', null, $_REQUEST['aepcargahoraria'] , $arrOp, $arrAttr );
		 		?>
	 		</div>
		<?php	
		}
		?>
		
		
		<center><button type="button" id="filtrar_pesquisa" onclick="filtraPesquisa( <?php echo $inuid;?>, '<?php echo $grupo;?>', <?php echo $prgid;?>, 'lista' )" class="btn btn-success salvar" <?php echo $disabled;?>><i class="fa fa-save"></i> Filtrar Pesquisa</button>
		<button type="button" onclick="filtraPesquisa( <?php echo $inuid;?>, '<?php echo $grupo;?>', <?php echo $prgid;?> , 'xls' )" class="btn btn-success salvar" <?php echo $disabled;?>> <i class="glyphicon glyphicon-th"></i> Gerar Planilha </button></center>
		
		<?php
	    if( ( $grupo != 'N'))
        {?>
			<hr class="divider">
			<div style="text-align: center;margin-top: 20px;">
				<b>Deseja selecionar a carga horária para as escolas?</b><br><br>
	            <?php 
	            	
		            if( ( ($chekedSim != '') || ($chekedNao != '') ) && ($arrCargaHoraria['rchresposta'] != '')   )
		            {
		            	 
		            	if( $itrid == 1 || $itrid == 3 )
		            	{
		            		if(($_REQUEST['epecod'] != '') || ($_REQUEST['epenome'] != '') || ($_REQUEST['checado'] != '')
		            				|| ($_REQUEST['aepcargahoraria'] != '') || ($_REQUEST['epemunicipio'] != ''))
		            		{
		            			$disabledCarga = 'disabled ';
		            		}
		            	}
		            	else
		            	{
		            		if(($_REQUEST['epecod'] != '') || ($_REQUEST['epenome'] != '') || ($_REQUEST['checado'] != '')
		            				|| ($_REQUEST['aepcargahoraria'] != '') )
		            		{
		            			$disabledCarga = 'disabled ';
		            		}
		            	}
		            }
		            
		            if ( ! (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
		            		in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
		            		in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
		            		in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) 
		            ) 
            		{
            			$bloqueiaSave = true;
            		}
					if($bloqueiaSave)
	                {
	                	$disabledCarga = 'disabled ';
	                    $displayescolas = '';
					}
					
					if((strtotime($proadesaoDados['pfadatafinal']. ' 12:00:00') < strtotime(date('Y-m-d G:i:s'))))
					{
						$bloqueiaSave = true;
						$disabledCarga = 'disabled ';
					}
	                ?>
	               <input type="radio" value="s" <?php echo $disabledCarga; echo $chekedSim;?> name="carga_horaria" id="carga_horaria_s" onclick="respostaCargaHoraria( <?php echo $inuid;?>, '<?php echo $grupo;?>', <?php echo $prgid;?>, 'lista', 'S' )" > Sim
	               <input type="radio" value="n" <?php echo $disabledCarga; echo $chekedNao;?> name="carga_horaria" id="carga_horaria_n" onclick="respostaCargaHoraria( <?php echo $inuid;?>, '<?php echo $grupo;?>', <?php echo $prgid;?>, 'lista', 'N' )" > Não
	        </div>
	        <?php
        }
		?>
			<hr class="divider">
		<div id="resultado"> 	
		<?php 
		$save = false;
       if( $listaInfoEscolas['status'] == 'success')
       {
       		$save = true;
       		$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
       }
       else
		{?>
        	<div style="" class="text-center " id="tb_render"><b>Nenhum registro encontrado</b></div>
            <?php 
         }?>
       </div>
       <?php 
       ?>
      <?php 
      
      $pfaid = 7; // proemi
      $modelAdesaoPrograma =  new Par3_Model_AdesaoPrograma();
      $result = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid);
      $docidInserido = $result['docid'];
      $situacaoAtual = wf_pegarEstadoAtual( $docidInserido);
      $esdid = $situacaoAtual['esdid'];
      if(($esdid) &&  ($esdid == WF_ESDID_ENVIADOPARAOMEC_PROEMI ))
      {
      		$arrParamExcecao['inuid'] = $inuid;
      		$arrParamExcecao['grupo'] = $grupo;
      		$arrParamExcecao['prgid'] = $prgid;
     		
     		$objModelExcecao = 	new Par3_Model_Excecaoescolasprograma();
      		$temSolicitacao = $objModelExcecao->possuiSolicitacaoPorGrupo($arrParamExcecao);
      
	      if($temSolicitacao == true)
	      {
	      	$bloqueiaSave = false;
	      }
      }
      //WF_ESDID_ENVIADOPARAOMEC_ABCMAIS = 6
      
      if((strtotime($proadesaoDados['pfadatafinal']. ' 12:00:00') < strtotime(date('Y-m-d G:i:s'))))
      {
      	$bloqueiaSave = true;
      }
      
      if( ( ! $bloqueiaSave )  && ( $grupo != 'N') && ($save) )
       {?> 
       <center><button type="button" class="btn btn-success salvar" onclick="submeteFormEscolas()"  <?php echo $disabled;?>><i class="fa fa-save"></i> Salvar</button></center>
       <?php 
       }?> 
	</form>
</div>

<?php die(); ?>