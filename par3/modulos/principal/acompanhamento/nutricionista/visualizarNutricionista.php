<?php $controllerVinculacao = new Par3_Controller_VinculacaoNutricionista(); ?>
<div class="modal-content animated fadeIn">
    <div class="ibox-title">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h3 class="center">Cadastro Nutricionista</h3>
    </div>
<?php 
if($dados->dancpf != '')
{
?>
    <div class="ibox-content">
        <div class="col-lg-offset-2">
        <div class="col-lg-6">
            <div class="row">
                <div class="col-lg-4 text-right">CPF:</div>
                <div class="col-lg-8 text-left"><?php echo formatar_cpf($dados->dancpf);?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Nome:</div>
                <div class="col-lg-8 text-left"><?php echo $usuario->usunome; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Data de Nascimento:</div>
                <div class="col-lg-8 text-left"><?php echo formata_data($dados->dancpf); ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Nome da Mãe:</div>
                <div class="col-lg-8 text-left"><?php echo $dados->dannomemae; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Sexo:</div>
                <div class="col-lg-8 text-left"><?php echo $dados->dansexo=='m' ? 'Masculino' : 'Feminino'; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">CRN/Região:</div>
                <div class="col-lg-8 text-left"><?php echo $dados->dancrn."/".$dados->dancrnuf; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Email principal:</div>
                <div class="col-lg-8 text-left"><?php echo $dados->danemailprincipal; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Email alternativo:</div>
                <div class="col-lg-8 text-left"><?php echo $dados->danemailalternativo; ?></div>
            </div>
        </div>

        <div class="col-lg-6">
            <div class="row">
                <div class="col-lg-4 text-right">Telefone fixo:</div>
                <div class="col-lg-8 text-left"><?php echo $dados->dantelefonefixo; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Telefone celular:</div>
                <div class="col-lg-8 text-left"><?php echo $dados->dantelefonecelular; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">CEP:</div>
                <div class="col-lg-8 text-left"><?php echo $endereco->endcep; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Logradouro:</div>
                <div class="col-lg-8 text-left"><?php echo $endereco->endlogradouro; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Complemento:</div>
                <div class="col-lg-8 text-left"><?php echo $endereco->endcomplemento; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Número:</div>
                <div class="col-lg-8 text-left"><?php echo $endereco->endnumero; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Bairro:</div>
                <div class="col-lg-8 text-left"><?php echo $endereco->endbairro; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">UF:</div>
                <div class="col-lg-8 text-left"><?php echo $dados->muncod; ?></div>
            </div>
            <div class="row">
                <div class="col-lg-4 text-right">Município:</div>
                <div class="col-lg-8 text-left"><?php echo $dados->estuf; ?></div>
            </div>
        </div>
        <br>
        <br>
    </div>
    </div>
<?php
}
else
{?>
	
    <table class="table">
      <tr class="warning text-center">
    <td>O Nutricionista ainda não preencheu seus dados pessoais</td>
    </tr>
    </table>
    
  
<?php 	
}
?>
        <div class="row">
            <div class="col-lg-offset-1 col-lg-10">
                <h4 class="center">Entidades Vinculadas:</h4>
                <?php
                
	                if($tipoId == 'cpf')
	                {
	                	$controllerVinculacao->listaVinculos(Array('vncpf'=>$danid), true);
	                }
	                else
	                {
	                	$controllerVinculacao->listaVinculos(Array('dancpf'=>$dados->dancpf), true);
	                }
                	 
                
                ?>
            </div>
        </div>
    </div>
</div>
