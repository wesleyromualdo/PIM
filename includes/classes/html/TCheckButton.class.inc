<?php
/**
 * classe TCheckButton
 * Classe para construção de botões de verificação
 */
class TCheckButton extends TField
{
	private $tag;
	
	function __construct($nome){
		parent::__construct($nome);
		
		$this->tag = new TElement("input");
	}
	
	function setChecked($check){
		$this->prop['checked'] = $check;
	}
	
	function getChecked(){
		return $this->prop['checked'];
	}	
	
    /**
     * método show()
     * exibe o widget na tela
     */
    public function show()
    {
        // atribui as propriedades da TAG
        $this->tag->name  = $this->name;    // nome da TAG
        $this->tag->value = $this->value;   // valor
        $this->tag->type  = 'checkbox';     // tipo do input
        
        // se o campo não é editável
        if ( parent::getHabil() == 'N' )
        {
            // desabilita a TAG input
            $this->tag->disabled = "disabled";
        }
		
        if ( $this->checked ){
        	$this->tag->checked = "checked";
        }
        
        if ( $this->acao ){
        	$this->tag->onclick = $this->acao;
        }
        
        // exibe a tag
        $this->tag->show();
    }
}
?>