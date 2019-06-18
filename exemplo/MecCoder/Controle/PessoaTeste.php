<?php
namespace Simec\Exemplo\Controle;

use \Simec\Corporativo\Mensagem\Mensagem as Mensagem;

/**
 * Classe controladora de fluxo
 * @author felipe.tcc@gmail.com
 */
class PessoaTeste extends \Simec\AbstractControle
{
    Const workflowPessoa = 350;
    
    use \Simec\Corporativo\Workflow\Controle\TraitWorkflow;
    
    public function finish()
    {
        
    }

    public function init()
    {
        switch (true)
        {
            case $this->post('requisicao') == 'salvarPessoa':
                $this->salvarPessoa();
                break;
            case $this->get('requisicao') == 'apagarPessoa':
                $this->apagarPessoa();
                break;
        }
    }

    public function listar()
    {
        $modelPessoa = new \Simec\Exemplo\Modelo\PessoaTeste\PessoaTeste();
        // Passando dados da lista para a View
        $this->toView('pesquisa', $this->post('pesquisa'));
        $this->toView('listaSql', $modelPessoa->listaSql($this->post('pesquisa')));
        $this->toView('listaCabecalho', ['Nome', 'Idade', 'Sexo', "Estado(s) Vinculado(s)"]);
        $this->toView('sqlEstado', \Territorios_Model_Estado::pegarSQLSelect($_POST));
        
        // inclui a view (com o mesmo nome do método, no caso: listar), podendo passar como parâmetro uma nova view.
        $this->showHtml();
    }
    
    /**
     * Método para validação de idade da pessoa (utilizado na validação de condição da ação do workflow).
     * @param integer $petid
     * @return mixed boolean|string
     */
    public function idadeValida($petid)
    {
        $msgRetorno = "A idade deve ser MAIOR do que 25.";
        if ($petid) {
            $modelPessoa = new \Simec\Exemplo\Modelo\PessoaTeste\PessoaTeste(["petid" => $petid]);
            $modelPessoa->lerObjeto();
            if ($modelPessoa->dados->petidade > 25) {
                $msgRetorno = true;
            }
        }
        
        return $msgRetorno;
    }

    /**
     * Método para enviar email (utilizado no PÓS-AÇÃO do workflow).
     * @param integer $petid
     */
    public function enviarEmailDevolucaoParaEmCadastramento($petid)
    {
//         Implementar a função do pós ação
    }

	public function cadastrar()
	{
	    $this->toView('arAba', $this->abaCadastroPessoa());
	    $this->toView('urlAba', 'exemplo.php?modulo=exemplo/controle/pessoa-teste/cadastrar');
	    
	    if (is_numeric($this->get('petid'))) {
    	    $modelPessoa = new \Simec\Exemplo\Modelo\PessoaTeste\PessoaTeste();
    	    $dadoPessoa = $modelPessoa->lerObjeto(
    	        (new \Simec\Exemplo\Dado\PessoaTeste\PessoaTeste(["petid" => $this->get('petid')]))
            );
    	    $dadoPessoa->estuf = $modelPessoa->pegarEstufAssociadoPorPetid($this->get('petid'));
    	    $this->toView('pessoa', $dadoPessoa);
    	    $this->toView('barraWorkflow', $this->WF_DesenharBarra($dadoPessoa->docid, ["petid" => $dadoPessoa->petid], 'Fluxo de pessoa'));
	    }
	    
	    $this->toView('sqlEstado', \Territorios_Model_Estado::pegarSQLSelect($_POST));
	    
	    $this->showHtml();
	}
	
	private function abaCadastroPessoa()
	{
	    $arAba = [
	        0 => ['descricao' => 'Lista de Pessoas', "link" => "exemplo.php?modulo=exemplo/controle/pessoa-teste/listar"],
	        1 => ['descricao' => 'Cadastro de Pessoas', "link" => "exemplo.php?modulo=exemplo/controle/pessoa-teste/cadastrar"],
	    ];
	    
	    return $arAba;
	}
	
	private function apagarPessoa()
	{
	    if (is_numeric($this->get('petid'))) {
    	    $arDado = ["petid"     => $this->get('petid'),
    	               "petstatus" => 'I'];
    	    $modelPessoa = new \Simec\Exemplo\Modelo\PessoaTeste\PessoaTeste($arDado);
    	    $modelPessoa->atualizar();
    	    
    	    Mensagem::registrarMensagemSucesso('', 'Operação realizada com sucesso!');
    	    $modelPessoa->commit();
	    } else {
    	    Mensagem::registrarMensagemErro('Faltou parâmetros', 'Operação não realizada!');	               
	    }
	    
	    header("Location: exemplo.php?modulo=exemplo/controle/pessoa-teste/listar");
	    die();
	}
	
	private function salvarPessoa()
	{
	    $arDado['petid']    = $this->get('petid');
	    $arDado['petnome']  = $this->post('nome');
	    $arDado['petidade'] = $this->post('idade');
	    $arDado['petsexo']  = $this->post('sexo');
	    $dadoPessoa = new \Simec\Exemplo\Dado\PessoaTeste\PessoaTeste($arDado);
	    
	    $modelPessoa = new \Simec\Exemplo\Modelo\PessoaTeste\PessoaTeste();
	    if (is_numeric($arDado['petid'])) {
	       $modelPessoa->atualizar($dadoPessoa);
	       $modelPessoa->deletarVinculoEstadoPorPetid($arDado['petid']);
	    } else {
	       $dadoDocumento = $this->WF_CriarDocumento(self::workflowPessoa, "Workflow de pessoa: " . $this->post('nome'));
	       $dadoPessoa->docid = $dadoDocumento->docid;
	       $modelPessoa->incluir($dadoPessoa);
	    }
	    
	    if (is_array($this->post('estuf')) && $dadoPessoa->petid) {
	        foreach ($this->post('estuf') as $estuf) {
	            if (empty($estuf)) {
	                continue;
	            }
	            $modelPessoa->incluir(new \Simec\Exemplo\Dado\PessoaTeste\PessoaTesteEstado([
	                "petid" => $dadoPessoa->petid,
	                "estuf" => $estuf
	            ]));
	        }
	    }
	    
	    Mensagem::registrarMensagemSucesso('', 'Operação realizada com sucesso!!');
	    $modelPessoa->commit();
	    
	    header("Location: exemplo.php?modulo=exemplo/controle/pessoa-teste/listar");
	    die();
	}
}
